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
#include <QHash>
#include <QBrush>
#include <QJsonObject>
#include "tower.h"
#include "enemy.h"
#include "projectile.h"
#include "hexmenu.h"
#include "hextile.h"
#include "InfoPanel.h"
#include <ICoreClass.h>
#include <ICoreObject.h>
#include <IScene.h>

class SceneGeneric : public QGraphicsScene,
                     public ICoreObject,
                     public IScene {
    Q_OBJECT
 public:
    Q_INVOKABLE SceneGeneric(QObject *parent,
                             QString objname,
                             QJsonObject &config);

    // IScene
    virtual bool isGoldAvailable(int gold) override { return goldCnt_ >= gold; }
    virtual void buildTower(QString &towerclass) override;
    virtual void addProjectile(Projectile *p) override;
    virtual QList<Waypoint> *getpRoute(QString name) override;
    virtual QPointF getHexCenter(int x, int y) override;

    virtual void gameLoop();

 protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

 signals:
    void signalUpdateGold(int gold);
    void signalUpdateLives(int lives);
    void signalUpdateWave(int wave);

 private:
    // Helper to turn grid columns/rows into exact screen pixel centers
    void setpTile(HexTile *tile, int x, int y);
    HexTile *getpTile(int x, int y);
    void resetCurrentHighlight();
    void sortEnemies();
    void closeActiveMenu();

 private:
    QJsonObject &cfg_;
    QJsonObject cfgmenu_;
    QList<Enemy *> enemies_;
    QList<TowerGeneric *> towers_;
    QList<Projectile *> projectiles_;
    QHash<int, HexTile *> tiles_;
    //QList<QPointF> visualPathPixelPoints;
    QHash<QString, QList<Waypoint>> routes_;
    HexMenu *hexmenu_;
    int hexHNum_;       // total HEX per row
    int hexVNum_;       // total HEX per col
    QGraphicsObject *currentHoveredHex_;
    QBrush oldBrush_;
    HexTile *hextileSelected_;
    InfoPanel *infoPanel_;
    int goldCnt_;
    int livesCnt_;
    int wavesCnt_;
};

DECLARE_CLASS(SceneGeneric)
