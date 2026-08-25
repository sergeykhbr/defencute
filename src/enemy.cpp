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

const qreal HEALTH_BAR_WIDTH = 30.0;
const qreal HEALTH_BAR_HEIGHT = 4.0;
const qreal HEALTH_BAR_OFFSET_Y = 10.0; // Distance above the enemy sprite boundary

Enemy::Enemy(const QList<QPointF>& points, QPointF startOffset) : QGraphicsPixmapItem(),
    speed_(2),
    healthMax_(100),
    startOffset_(startOffset),
    traveledDistance_(0) {
    pathPoints = points;
    spriteSheet_.load(":/images/base_walk_strip8.png");

    setOffset(-FRAME_WIDTH / 2, -FRAME_HEIGHT / 2);

    resetPosition();
}

void Enemy::resetPosition() {
    currentWaypointIndex = 0;
    health_ = healthMax_;
    currentFrameIndex_ = 0;
    animationTimer_ = 0;
    directionRow_ = 0;
    traveledDistance_ = 0;

    if (!pathPoints.isEmpty()) {
        setPos(pathPoints[0] + startOffset_);
    }
    updateVisualFrame();
}

void Enemy::updateVisualFrame() {
    if (spriteSheet_.isNull()) {
        return;
    }

    // Math out source pixels inside the image asset grid
    int srcX = currentFrameIndex_ * FRAME_WIDTH;
    int srcY = directionRow_ * FRAME_HEIGHT;

    // Crop the single active frame out of the master image sheet
    QPixmap singleFrame = spriteSheet_.copy(srcX, srcY, FRAME_WIDTH, FRAME_HEIGHT);
        
    // Push the cropped picture to Qt's rendering system
    setPixmap(singleFrame);
}

QVector2D Enemy::getFuturePos(int tick) {
    QPointF target;
    QVector2D targetPos;
    QVector2D direction;
    qreal distance;

    int tmpWaypoint = currentWaypointIndex;
    if (tmpWaypoint >= pathPoints.size()) {
        tmpWaypoint = pathPoints.size() - 1;
    }

    QVector2D currentPos(x(), y());
    target = pathPoints[tmpWaypoint] + startOffset_;
    for (int i = 0; i < tick; i++) {
        targetPos = QVector2D(target.x(), target.y());
        direction = targetPos - currentPos;
        distance = direction.length();
        if (distance <= speed_) {
            currentPos = targetPos;
            if (++tmpWaypoint >= pathPoints.size()) {
                tmpWaypoint = pathPoints.size() - 1;
            }
            target = pathPoints[tmpWaypoint] + startOffset_;
        } else {
            direction.normalize();
            QVector2D step(direction.x() * speed_,
                           direction.y() * speed_ * HEX_X_TO_Y);
            currentPos += step;
        }
    }
    return currentPos;
}

void Enemy::move() {
    if (currentWaypointIndex >= pathPoints.size()) {
        return;
    }

    QPointF target = pathPoints[currentWaypointIndex] + startOffset_;
    QVector2D currentPos(x(), y());
    QVector2D targetPos(target.x(), target.y());
        
    // Calculate vector direction and distance to the next hex center
    QVector2D direction = targetPos - currentPos;
    qreal distance = direction.length();

    if (distance <= speed_) {
        // Close enough! Snap to target and aim for the next waypoint
        setPos(target);
        traveledDistance_ += distance;
        currentWaypointIndex++;
    } else {
        // Step forward along the direction vector
        direction.normalize();
        QVector2D step(direction.x() * speed_,
                       direction.y() * speed_ * HEX_X_TO_Y);
        moveBy(step.x(), step.y());
        traveledDistance_ = step.length();

        setZValue(y());
    }

    animationTimer_++;
    if (animationTimer_ >= ANIM_SPEED_TICKS) {
        animationTimer_ = 0;
        currentFrameIndex_ = (currentFrameIndex_ + 1) % 3; // Loop between frames 0, 1, 2
        updateVisualFrame();
    }
}

bool Enemy::hasReachedEnd() const {
    return currentWaypointIndex >= pathPoints.size();
}

void Enemy::takeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) {
        health_ = 0;
    }
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter, option, widget); 

    if (health_ <= 0) {
        return;
    }

    qreal healthRatio = static_cast<qreal>(health_) / healthMax_;

    // Use your base class pixmap size configurations to place the health bar perfectly
    qreal spriteWidth = this->pixmap().width();
    
    // Center the bar over the texture width bounding space
    qreal barX = (spriteWidth - HEALTH_BAR_WIDTH) / 2.0 - (spriteWidth / 2.0); 
    qreal barY = -(this->pixmap().height() / 2.0) - HEALTH_BAR_OFFSET_Y - HEALTH_BAR_HEIGHT;

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
}

// Ensure the item bounding space allocation covers the extra vertical height of the bar
QRectF Enemy::boundingRect() const {
    // 1. Get the standard base image bounding rectangle
    QRectF baseRect = QGraphicsPixmapItem::boundingRect();
    
    // 2. Expand the top boundary upward to fit the health bar safely
    // adjusting the top coordinate y by adding negative padding space
    baseRect.setTop(baseRect.top() - HEALTH_BAR_OFFSET_Y - HEALTH_BAR_HEIGHT - 5);
    
    return baseRect;}
