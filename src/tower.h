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

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector2D>
#include <QList>
#include "enemy.h"

class Tower : public QGraphicsPixmapItem {
 public:
    Tower(QPointF pos,  QGraphicsScene* scene);

    virtual void updateCooldown();
    virtual void updateTarget(Enemy* enemy);
    virtual int getProjectileFramesToTarget() { return 25; }
    virtual int getCooldownTime() { return 75; }
    virtual int getRange() { return 150; }

 private:
    QGraphicsScene* scene_;
    QPointF hexCenter_;
    QPixmap spriteSheet_;
    int attackCooldown_;
};
