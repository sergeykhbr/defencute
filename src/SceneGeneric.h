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
#include "projectile.h"
#include "hexmenu.h"
#include "hextile.h"

struct Waypoint {
    int col;
    int row;
};

class SceneGeneric : public QGraphicsScene {
    Q_OBJECT
 public:
    SceneGeneric(QObject *parent = nullptr);

    virtual void gameLoop();
    virtual void addProjectile(Projectile *p);

 protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

 private slots:
    void slotBuildTower(const QString &towerName);

 private:
    // Helper to turn grid columns/rows into exact screen pixel centers
    void resetCurrentHighlight();
    QPointF getHexCenter(int col, int row);
    void sortEnemies();
    void closeActiveMenu();

 private:
    QList<Enemy *> enemies_;
    QList<TowerGeneric *> towers_;
    QList<Projectile *> projectiles_;
    QList<QPointF> visualPathPixelPoints;
    HexMenu *hexmenu_;
    int hexHNum_;       // total HEX per row
    int hexVNum_;       // total HEX per col
    QGraphicsPolygonItem *currentHoveredHex_;
    QBrush oldBrush_;
    HexTile *hextileSelected_;
};
