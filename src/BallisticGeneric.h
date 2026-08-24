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

#include <QGraphicsLineItem>
#include <QVector2D>
#include <cmath>
#include "enemy.h"

class BallisticGeneric : public QGraphicsLineItem {
 public:
    bool isDead = false;
    int damage = 25;

    BallisticGeneric(QPointF start,
                     QPointF target,
                     Enemy* enemy,
                     qreal speed);

    void advanceFrame();
    int getFlightTime() { return static_cast<int>(1.0 / speed_); }

 private:
    void updateTrajectory();

 private:
    QPointF startPos;
    QPointF targetPos;
    Enemy* targetEnemy;
    
    qreal progress = 0.0;       // Goes from 0.0 (launch) to 1.0 (impact)
    qreal speed_;               // Controls how fast the arrow flies (approx 25 frames total)
    qreal maxHeight = 60.0;     // Peak altitude of the ballistic arc in pixels
};
