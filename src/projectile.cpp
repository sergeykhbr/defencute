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

#include "projectile.h"
#include <QPainter>

Projectile::Projectile(QPointF start,
                       QPointF target,
                       IEnemy* enemy,
                       qreal speed,
                       int damage) 
    : QGraphicsItem(),
    startPos_(start),
    targetPos_(target),
    targetEnemy_(enemy),
    progress_(0),
    speed_(1.0/speed),
    damage_(damage),
    isDead_(false)
{
}

void Projectile::advanceFrame() {
    if (isDead_) {
        return;
    }

    progress_ += speed_;
    if (progress_ >= 1.0) {
        progress_ = 1.0;
        isDead_ = true;
            
        // Deal damage on impact if the enemy is still alive
        if (targetEnemy_) {
            targetEnemy_->takeDamage(damage_);
        }
    } else {
        updateTrajectory();
    }
}

////////////////////
// Ballistic arrow
Arrow::Arrow(QPointF start,
            QPointF target,
            IEnemy* enemy,
            qreal speed,
            int damage)
    : Projectile(start, target, enemy, speed, damage),
    maxHeight_(60.0),
    angleDegrees_(0)
{
    updateTrajectory();
}

void Arrow::updateTrajectory() {
    // 1. Math out current flat X/Y coordinates on the ground plane
    qreal currentX = startPos_.x() + (targetPos_.x() - startPos_.x()) * progress_;
    qreal currentY = startPos_.y() + (targetPos_.y() - startPos_.y()) * progress_;

    // 2. Compute 3D Altitude (Z) using a clean parabolic arch equation
    qreal currentZ = 4.0 * maxHeight_ * progress_ * (1.0 - progress_);

    // 3. Project 3D into 2D Screen Space (Subtract Z from Y to lift it up)
    qreal screenX = currentX;
    qreal screenY = currentY - currentZ;

    setPos(screenX, screenY);

    // 4. Visual Flavor: Rotate the arrow so it faces the direction of the arc
    // Calculate the trajectory tangent angle to make the arrow tip point down on descent
    qreal nextProgress = qMin(progress_ + 0.01, 1.0);
    qreal nextX = startPos_.x() + (targetPos_.x() - startPos_.x()) * nextProgress;
    qreal nextY = startPos_.y() + (targetPos_.y() - startPos_.y()) * nextProgress;
    qreal nextZ = 4.0 * maxHeight_ * nextProgress * (1.0 - nextProgress);
        
    qreal dx = nextX - currentX;
    qreal dy = (nextY - nextZ) - (currentY - currentZ);
    angleDegrees_ = std::atan2(dy, dx) * 180.0 / M_PI;
}

void Arrow::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
    painter->save();
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::transparent);
    painter->rotate(angleDegrees_);
    painter->drawLine(0, 0, 4, 0);
    painter->restore();
}

QRectF Arrow::boundingRect() const {
    return QRectF(-4, -4, 4, 4); // Small bounding box for a round bullet
}

///////////////////////
// Bullet
Bullet::Bullet(QPointF start,
            QPointF target,
            IEnemy* enemy,
            qreal speed,
            int damage)
    : Projectile(start, target, enemy, speed, damage) {
    dx_ = (target.x() - start.x());
    dy_ = (target.y() - start.y());
    setZValue(1000); // Always render bullets on top of everything
    updateTrajectory();
}

void Bullet::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawEllipse(boundingRect());
}

QRectF Bullet::boundingRect() const {
    return QRectF(-4, -4, 8, 8); // Small bounding box for a round bullet
}

void Bullet::updateTrajectory() {
    qreal nextX = startPos_.x() + progress_ * dx_;
    qreal nextY = startPos_.y() + progress_ * dy_;
    setPos(nextX, nextY);
}
