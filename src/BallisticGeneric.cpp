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

#include "BallisticGeneric.h"

BallisticGeneric::BallisticGeneric(QPointF start,
                                   QPointF target,
                                   Enemy* enemy,
                                   qreal speed) 
    : QGraphicsLineItem(),
    startPos(start),
    targetPos(target),
    targetEnemy(enemy),
    speed_(speed)
{
    // Draw a simple 2D line representing a sleek arrow shaft
    setPen(QPen(Qt::black, 2));
    updateTrajectory();
}

void BallisticGeneric::advanceFrame() {
    if (isDead) return;

    progress += speed_;
    if (progress >= 1.0) {
        progress = 1.0;
        isDead = true;
            
        // Deal damage on impact if the enemy is still alive
        if (targetEnemy && targetEnemy->getHealth() > 0) {
            targetEnemy->takeDamage(damage);
        }
        return;
    }

    updateTrajectory();
}

void BallisticGeneric::updateTrajectory() {
    // 1. Math out current flat X/Y coordinates on the ground plane
    qreal currentX = startPos.x() + (targetPos.x() - startPos.x()) * progress;
    qreal currentY = startPos.y() + (targetPos.y() - startPos.y()) * progress;

    // 2. Compute 3D Altitude (Z) using a clean parabolic arch equation
    qreal currentZ = 4.0 * maxHeight * progress * (1.0 - progress);

    // 3. Project 3D into 2D Screen Space (Subtract Z from Y to lift it up)
    qreal screenX = currentX;
    qreal screenY = currentY - currentZ;

    setPos(screenX, screenY);

    // 4. Visual Flavor: Rotate the arrow so it faces the direction of the arc
    // Calculate the trajectory tangent angle to make the arrow tip point down on descent
    qreal nextProgress = qMin(progress + 0.01, 1.0);
    qreal nextX = startPos.x() + (targetPos.x() - startPos.x()) * nextProgress;
    qreal nextY = startPos.y() + (targetPos.y() - startPos.y()) * nextProgress;
    qreal nextZ = 4.0 * maxHeight * nextProgress * (1.0 - nextProgress);
        
    qreal dx = nextX - currentX;
    qreal dy = (nextY - nextZ) - (currentY - currentZ);
    qreal angleDegrees = std::atan2(dy, dx) * 180.0 / M_PI;

    // Point the drawn line forward
    setLine(0, 0, 12, 0); 
    setRotation(angleDegrees);
}
