/*
 *  Copyright 2026 Sergey Khabarov, sergeykhbr@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include "common.h"
#include "enemy.h"

const int FRAME_WIDTH = 96;  // Size of one frame box
const int FRAME_HEIGHT = 64;
const int ANIM_SPEED_TICKS = 8; // Change sprite frame every 8 game loop ticks (~60fps / 8)

const int HEALTH_BAR_WIDTH = 30;
const int HEALTH_BAR_HEIGHT = 4;
const int HEALTH_BAR_OFFSET_Y = 2; // Distance above the enemy sprite boundary

Enemy::Enemy(QObject *parent,
             QString objname,
             QJsonObject &cfg)
    : QGraphicsObject(),
    ICoreObject(objname),
    cfg_(cfg),
    speed_(2),
    healthMax_(100),
    reward_(10),
    traveledDistance_(0)
{
    registerInterface(static_cast<IEnemy *>(this));

    ICore * icore = getpCoreInterface();
    QString scenename = cfg["scene"].toString();
    iscene_ = dynamic_cast<IScene *>(icore->getpObjInterface(scenename, "IScene"));

    route_ = iscene_->getpRoute(cfg["Route"].toString());
    startOffset_ = QPointF(cfg["offx"].toInt(),
                           cfg["offy"].toInt());
    spriteSheet_.load(":/images/base_walk_strip8.png");
    singleFrame_ = spriteSheet_.copy(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

    resetPosition();
}

void Enemy::resetPosition() {
    currentWaypointIndex_ = 0;
    health_ = healthMax_;
    currentFrameIndex_ = 0;
    animationTimer_ = 0;
    directionRow_ = 0;
    traveledDistance_ = 0;
    Waypoint wp0 = route_->at(currentWaypointIndex_);
    curpos_ = wp0.pos + QVector2D(startOffset_);

    distanceToTick(wp0.dist, tickPerStep_, tickDistance2D_);

    setPos(curpos_.toPointF());
    updateVisualFrame();
}

void Enemy::distanceToTick(QVector2D dist,
                           int &tickPerStep,
                           QVector2D &inc) {
    // real distance is higher than visible
    dist.setY(dist.y() / HEX_X_TO_Y);

    qreal totalDist = dist.length();
    if (totalDist == 0) {
        tickPerStep = 1;
        inc = QVector2D(0, 0);
        return;
    }
    tickPerStep = static_cast<int>(totalDist / speed_ + 0.5);
    inc = dist / tickPerStep;
    // scale back
    inc.setY(inc.y() * HEX_X_TO_Y);
}


void Enemy::updateVisualFrame() {
    if (spriteSheet_.isNull()) {
        return;
    }

    // Math out source pixels inside the image asset grid
    int srcX = currentFrameIndex_ * FRAME_WIDTH;
    int srcY = directionRow_ * FRAME_HEIGHT;

    // Crop the single active frame out of the master image sheet
    singleFrame_ = spriteSheet_.copy(srcX, srcY, FRAME_WIDTH, FRAME_HEIGHT);
}

QVector2D Enemy::getFuturePos(int tick) {
    int tmpWaypoint = currentWaypointIndex_;
    if (tmpWaypoint >= route_->size()) {
        tmpWaypoint = route_->size() - 1;
    }

    int tickPerStep = tickPerStep_;
    QVector2D tcurpos = curpos_;
    QVector2D ttickDistance2D = tickDistance2D_;

    for (int i = 0; i < tick; i++) {
        tcurpos += ttickDistance2D;

        if (--tickPerStep <= 0) {
            if (++tmpWaypoint < route_->size()) {
                Waypoint wp0 = route_->at(tmpWaypoint);
                tcurpos = wp0.pos + QVector2D(startOffset_);

                distanceToTick(wp0.dist, tickPerStep, ttickDistance2D);
            }
        }
    }
    return tcurpos;
}

void Enemy::move() {
    if (currentWaypointIndex_ >= route_->size()) {
        return;
    }

    curpos_ += tickDistance2D_;
    if (--tickPerStep_ <= 0) {
        if (++currentWaypointIndex_ < route_->size()) {
            Waypoint wp0 = route_->at(currentWaypointIndex_);
            curpos_ = wp0.pos + QVector2D(startOffset_);

            distanceToTick(wp0.dist, tickPerStep_, tickDistance2D_);
        }
    }
    setPos(curpos_.toPointF());
    setZValue(curpos_.y());

    animationTimer_++;
    if (animationTimer_ >= ANIM_SPEED_TICKS) {
        animationTimer_ = 0;
        currentFrameIndex_ = (currentFrameIndex_ + 1) % 3; // Loop between frames 0, 1, 2
        updateVisualFrame();
    }
}

bool Enemy::hasReachedEnd() const {
    return currentWaypointIndex_ >= route_->size();
}

void Enemy::takeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) {
        health_ = 0;
    }
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    int w = singleFrame_.width();
    int h = singleFrame_.height();
    painter->drawPixmap(-w/2, -h/2, singleFrame_);

    if (health_ <= 0) {
        return;
    }

    qreal healthRatio = static_cast<qreal>(health_) / healthMax_;

    // Center the bar over the texture width bounding space
    int barX = -HEALTH_BAR_WIDTH / 2; 
    int barY = -(h/2 + HEALTH_BAR_OFFSET_Y + HEALTH_BAR_HEIGHT);

    // Background Layer (Red Container Fill)
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawRect(QRectF(barX, barY, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT));

    // Foreground Layer (Green Value Fill)
    painter->setBrush(Qt::green);
    painter->drawRect(QRectF(barX, barY, HEALTH_BAR_WIDTH * healthRatio, HEALTH_BAR_HEIGHT));

    // Border Outline Frame
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(QRectF(barX, barY, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT));

#if 0
    // Debug draw boundRect:
    painter->save();
    QPen debugPen(QColor(255, 0, 100, 220), 1.5, Qt::DashLine);
    debugPen.setCosmetic(true); // THE SECRET: Prevents line scaling when zooming the view camera
    painter->setPen(debugPen);
    painter->setBrush(QColor(255, 0, 100, 20)); // Soft translucent red fill back drop highlight
    painter->drawRect(this->boundingRect());
    painter->restore();
#endif
}

// Ensure the item bounding space allocation covers the extra vertical height of the bar
QRectF Enemy::boundingRect() const {
    // Get the standard base image bounding rectangle
    int w = singleFrame_.width();
    int h = singleFrame_.height();
    int top = HEALTH_BAR_OFFSET_Y + HEALTH_BAR_HEIGHT + 5;
    QRectF baseRect(-w/2, -h/2, w, h);
    // increase top border:
    baseRect.setTop(baseRect.top() - top);
    return baseRect;
}
