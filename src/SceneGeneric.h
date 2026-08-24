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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QBrush>
#include "tower.h"
#include "enemy.h"
#include "BallisticGeneric.h"

struct Waypoint {
    int col;
    int row;
};

class SceneGeneric : public QGraphicsScene {
 public:
    SceneGeneric(QObject *parent = nullptr);

    virtual void gameLoop();
    virtual void addBallistic(BallisticGeneric *p);

 protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

 private:
    // Helper to turn grid columns/rows into exact screen pixel centers
    void resetCurrentHighlight();
    QPointF getHexCenter(int col, int row);

 private:
    enum EHexDataRole {
        HexPathRole,
        HexCenter,
        HexUnavailbleRole,
        HexTowerRole,
        HexTowerType,
        HexTowerLevel
    };
    

    Enemy* enemy_[2];
    QList<Tower*> towers;
    QList<QPointF> visualPathPixelPoints;
    QList<BallisticGeneric *> listBallistic_;
    int hexHNum_;       // total HEX per row
    int hexVNum_;       // total HEX per col
    QGraphicsPolygonItem *currentHoveredHex_;
    QBrush oldBrush_;
};
