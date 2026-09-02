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

#include <IFace.h>
#include <QList>
#include <QPointF>
#include <QVector2D>
#include "../projectile.h"

struct Waypoint {
    int idx;
    QVector2D pos;
    QVector2D dist;
};

//static const int UserBuildBarricade = 0;
//static const int UserBoostTower = 1;
//static const int UserDistraction = 2;

class IScene : public Interface {
 public:
    IScene() : Interface("IScene") {}

    virtual bool isGoldAvailable(int gold) = 0;
    virtual int getUserActionTimeout(QString &type) = 0;
    virtual void buildTower(QString &towerclass) = 0;
    virtual void addProjectile(Projectile *p) = 0;
    virtual QList<Waypoint> *getpRoute(QString name) = 0;
    virtual QPointF getHexCenter(int x, int y) = 0;
    virtual void unitKilled(int reward) = 0;
    virtual void unitPassed() = 0;
};


