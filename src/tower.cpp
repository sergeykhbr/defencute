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

#include "tower.h"
#include "BallisticGeneric.h"
#include "SceneGeneric.h"

Tower::Tower(QPointF pos,  QGraphicsScene *scene) :
    QGraphicsPixmapItem(),
    scene_(scene),
    hexCenter_(pos)
{
    spriteSheet_.load(":/images/twr1.png");
    QPixmap singleFrame = spriteSheet_.copy(0, 0, 96, 64);
    setPixmap(singleFrame);

    setZValue(pos.y());
    setPos(pos.x() - 96/2, pos.y() - 64/2);
    attackCooldown_ = 0;
}

void Tower::updateCooldown() {
    if (attackCooldown_ > 0) {
        --attackCooldown_;
    }
}

void Tower::updateTarget(Enemy* enemy) {
    if (attackCooldown_ > 0) {
        return;
    }

    if (!enemy || enemy->getHealth() <= 0) {
        return;
    }

    // Calculate distance to enemy
    QVector2D towerPos(hexCenter_);
    QVector2D enemyPos = enemy->getFuturePos(getProjectileFramesToTarget());
    qreal distance = towerPos.distanceToPoint(enemyPos);

    // Shoot if enemy is in range and cooldown is ready
    if (distance > getRange()) {
        return;
    }

    QPointF targetPoint(enemyPos.x(), enemyPos.y());

    // Create ballistic projectile and add to world container
    BallisticGeneric* newArrow = 
        new BallisticGeneric(hexCenter_,
                             targetPoint,
                             enemy,
                             1.0 / getProjectileFramesToTarget());
    (dynamic_cast<SceneGeneric *>(scene_))->addBallistic(newArrow);
    attackCooldown_ = getCooldownTime();
}
