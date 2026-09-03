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
#include "actionbtn.h"
#include "InfoPanel.h"
#include <ICoreClass.h>
#include <ICoreObject.h>
#include <IScene.h>
#include <vector>

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
    virtual int getUserActionTimeout(QString &type) override;
    virtual void buildTower(QString &towerclass) override;
    virtual void addProjectile(Projectile *p) override;
    virtual QList<Waypoint> *getpRoute(QString name) override;
    virtual QPointF getHexCenter(int x, int y) override;
    virtual void unitKilled(int reward) override;
    virtual void unitPassed() override;

    virtual void gameLoop();

 protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

 signals:
    void signalUpdateGold(int gold);
    void signalUpdateLives(int lives);
    void signalUpdateWave(int wave);

 protected slots:
    void slotBuildBarricadeRequest();
    void slotBoostTowerRequest();
    void slotDistractRequest();

 private:
    enum ESpawnType {
        SpawnEventUser,
        SpawnEventTimeout,
        SpawnEventWaveEnd,
        SpawnGroup
    };
    struct SpawnUnitType {
        int dt;
        int offx;
        int offy;
        QString enemyClass;
        QString routeName;
    };
    struct SpawnEventType {
        ESpawnType type;
        QString name;
        int spawnCnt;
        int spawnPeriod;
        int respawnTotal;
        std::vector<SpawnUnitType> units;
    };

    void addWave(QJsonObject wave, SpawnEventType *ev);
    // Helper to turn grid columns/rows into exact screen pixel centers
    void setpTile(HexTile *tile, int x, int y);
    HexTile *getpTile(int x, int y);
    void resetCurrentHighlight();

    void updateScenario();
    void updateEnemies();
    void sortEnemies();
    void cleanupEnemies();
    void updateTowers();
    void updateProjectiles();
    void closeActiveMenu();

 private:
    QJsonObject &cfg_;
    QJsonObject cfgmenu_;
    QList<Enemy *> enemies_;
    QList<TowerGeneric *> towers_;
    QList<Projectile *> projectiles_;
    QHash<int, HexTile *> tiles_;
    QHash<QString, QList<Waypoint>> routes_;
    HexMenu *hexmenu_;
    int hexHNum_;       // total HEX per row
    int hexVNum_;       // total HEX per col
    QGraphicsObject *currentHoveredHex_;
    ActionButton *actions_[3];
    QBrush oldBrush_;
    HexTile *hextileSelected_;
    InfoPanel *infoPanel_;
    int goldCnt_;
    int livesCnt_;
    int wavesCnt_;
    int barricadeCnt_;
    int barricadeTimeout_;

    int scenarioPos_;
    std::vector<SpawnEventType> scenario_;
    std::list<SpawnEventType> activeGroups_;
};

DECLARE_CLASS(SceneGeneric)
