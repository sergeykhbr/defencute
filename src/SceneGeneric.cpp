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

#include <common.h>
#include <ICore.h>
#include "SceneGeneric.h"
#include <algorithm>
#include <QJsonArray>

const qreal HEX_HEIGHT = std::sqrt(3) * HEX_RADIUS_Y;
const qreal HORIZ_SPACING = 1.5 * HEX_RADIUS_X;


SceneGeneric::SceneGeneric(QObject *parent,
                            QString objname,
                            QJsonObject &config)
    : QGraphicsScene(parent),
    ICoreObject(objname),
    cfg_(config),
    hextileSelected_(nullptr),
    goldCnt_(100),
    livesCnt_(20),
    wavesCnt_(20)
{
    registerInterface(static_cast<IScene *>(this));
    cfg_["scene"] = getObjName();

    currentHoveredHex_ = nullptr;
    setSceneRect(0, 0, 800, 600);

    hexHNum_ = static_cast<int>(width() / HORIZ_SPACING) + 1;
    hexVNum_ = static_cast<int>(height() / HEX_HEIGHT) + 1;

    int ztile = cfg_["ZDepth"].toObject()["HexTile"].toInt();
    for (int col = 0; col < hexHNum_; ++col) {
        for (int row = 0; row < hexVNum_; ++row) {
            QPointF center = getHexCenter(col, row);
            HexTile *hex = new HexTile(center, ztile);
            setpTile(hex, col, row);
            addItem(hex);
        }
    }
    // high-light the path tiles
    QJsonArray routes = cfg_.value("Routes").toArray();
    for (auto route : routes) {
        Waypoint wp0;
        Waypoint wp1;
        QJsonObject routeObj = route.toObject();
        QString rname = routeObj["RouteName"].toString();
        int x = routeObj["StartX"].toInt();
        int y = routeObj["StartY"].toInt();
        int dx = 0;
        int dy = 0;
        int N = 0;
        wp0.idx = 0;
        wp0.pos = QVector2D(getHexCenter(x, y));
        getpTile(x, y)->changeState(ITile::TileRoute);

        QJsonArray routeSteps = routeObj.value("Steps").toArray();
        for (auto step : routeSteps) {
            QJsonObject stepObj = step.toObject();
            N = stepObj["N"].toInt();
            dx = stepObj["dx"].toInt();
            dy = stepObj["dy"].toInt();
            for (int i = 0; i < N; i++) {
                x += dx;
                y += dy;
                getpTile(x, y)->changeState(ITile::TileRoute);
            }
            wp1.idx = wp0.idx + 1;
            wp1.pos = QVector2D(getHexCenter(x, y));
            wp1.dist = QVector2D(0, 0);
            wp0.dist = wp1.pos - wp0.pos;
            routes_[rname].append(wp0);
            wp0 = wp1;
        }
        routes_[rname].append(wp0);
    }


    hexmenu_ = new HexMenu(cfg_);
    addItem(hexmenu_);

    // Information panel: health, wave number:
    infoPanel_ = new InfoPanel(static_cast<IScene *>(this));
    infoPanel_->setPos(0, 0);
    addItem(infoPanel_);

    connect(this, &SceneGeneric::signalUpdateGold,
            infoPanel_, &InfoPanel::slotUpdateGold);

    connect(this, &SceneGeneric::signalUpdateLives,
            infoPanel_, &InfoPanel::slotUpdateLives);

    connect(this, &SceneGeneric::signalUpdateWave,
            infoPanel_, &InfoPanel::slotUpdateWave);

    emit signalUpdateGold(goldCnt_);
    emit signalUpdateLives(livesCnt_);
    emit signalUpdateWave(wavesCnt_);

    // Spawing scenario:
    spawnGroup_.spawnTotal = 10;
    spawnGroup_.spawnPeriod = 60 * 10;
    spawnGroup_.spawnCnt = 0;
    spawnGroup_.unit[0].deltaTime = 2;
    spawnGroup_.unit[0].enemyClass = "Enemy";
    spawnGroup_.unit[0].offx = 10;
    spawnGroup_.unit[0].offy = -5;

    spawnGroup_.unit[1].deltaTime = 120;
    spawnGroup_.unit[1].enemyClass = "Enemy";
    spawnGroup_.unit[1].offx = -15;
    spawnGroup_.unit[1].offy = 5;
}

void SceneGeneric::setpTile(HexTile *tile, int x, int y) {
    int h = (x << 16) | (y & 0xFFFF);
    tiles_[h] = tile;
}

HexTile *SceneGeneric::getpTile(int x, int y) {
    int h = (x << 16) | (y & 0xFFFF);
    return tiles_[h];
}

void SceneGeneric::resetCurrentHighlight() {
    if (currentHoveredHex_) {
        qobject_cast<HexTile *>(currentHoveredHex_)->selectOff();
        currentHoveredHex_ = nullptr;
    }
}

void SceneGeneric::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseMoveEvent(event);

    // Find the exact item under the cursor
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    if (!item) {
        resetCurrentHighlight();
        return;
    }
    QGraphicsObject *graphicObj = item->toGraphicsObject();
    if (!graphicObj) {
        resetCurrentHighlight();
        return;
    }
    HexTile *hex = qobject_cast<HexTile *>(graphicObj);

    // Check if it's one of your valid hex tiles
    if (hex && hex->isBuildAvailable()) {
        if (hex != currentHoveredHex_) {
            // Restore the previous hexagon's original color
            resetCurrentHighlight();
            hex->selectOn();
            currentHoveredHex_ = hex;
        }
    } else {
        // Mouse moved over empty space or a non-hex item
        resetCurrentHighlight();
    }
}

void SceneGeneric::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mousePressEvent(event);

    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    QGraphicsObject *graphicObj = item->toGraphicsObject();

    closeActiveMenu();

    if (graphicObj) {
        if (qobject_cast<TowerGeneric *>(graphicObj)) {
            bool st = true;
        } else if (qobject_cast<HexTile *>(graphicObj)) {
            hextileSelected_ = qobject_cast<HexTile *>(graphicObj);
            if (hextileSelected_->isBuildAvailable()) {
                hexmenu_->setPos(event->scenePos());
                hexmenu_->setVisible(true);
            }
        }
    }
}

QList<Waypoint> *SceneGeneric::getpRoute(QString name) {
    auto it = routes_.find(name);
    if (it == routes_.end()) {
        return nullptr;
    }
    return &(*it);
}

void SceneGeneric::closeActiveMenu() {
    hexmenu_->setVisible(false);
}

void SceneGeneric::buildTower(QString &towerclass) {
    ICore *core = getpCoreInterface();
    QJsonValue towers = cfg_["towers"];
    // create copy of tower config for a new building plus position:
    QJsonObject twrcfg = towers.toObject()[towerclass].toObject();
    twrcfg["scene"] = cfg_["scene"];
    twrcfg["posx"] = hextileSelected_->getCenter().x();
    twrcfg["posy"] = hextileSelected_->getCenter().y();
    twrcfg["MenuZDepth"] = cfg_["ZDepth"].toObject()["TowerMenu"].toInt();
    TowerGeneric *newTower = dynamic_cast<TowerGeneric *>(
        core->createQtClassObject(this, towerclass, twrcfg));

    if (newTower) {
        addItem(newTower);
        towers_.append(newTower);
        newTower->attachTile(static_cast<ITile *>(hextileSelected_));

        goldCnt_ -= newTower->getPrice();
        emit signalUpdateGold(goldCnt_);
    }
}

// Helper to turn grid columns/rows into exact screen pixel centers
QPointF SceneGeneric::getHexCenter(int col, int row) {
    qreal posX = col * HORIZ_SPACING + HEX_RADIUS_X;
    qreal posY = row * HEX_HEIGHT + (HEX_HEIGHT / 2.0);
    if (col % 2 != 0) {
        posY += HEX_HEIGHT / 2.0;
    }
    return QPointF(posX, posY);
}

void SceneGeneric::addProjectile(Projectile *p) {
    projectiles_.append(p);
    addItem(p);
}

void SceneGeneric::updateEnemies() {
    // Enemy update positions
    for (Enemy *enemy : enemies_) {
        enemy->move();
    }

    // Spawn new enemies:
    if (++spawnGroup_.spawnCnt >= spawnGroup_.spawnPeriod) {
        spawnGroup_.spawnCnt = 0;
    }
    for (int i = 0; i < 2; i++) {
        SpawType *unit = &spawnGroup_.unit[i];
        if (unit->deltaTime == spawnGroup_.spawnCnt) {
            QJsonObject ecfg;
            ecfg["Route"] = "route1";
            ecfg["offx"] = unit->offx;
            ecfg["offy"] = unit->offy;
            ecfg["scene"] = getObjName();
            Enemy *enemy = new Enemy(nullptr, unit->enemyClass, ecfg);
            addItem(enemy);
            enemies_.push_back(enemy);
        }
    }
}

void SceneGeneric::sortEnemies() {
    if (enemies_.isEmpty()) {
        return;
    }
    // index [0] is always the lead unit
    std::sort(enemies_.begin(),
              enemies_.end(),
              [](const Enemy *a, const Enemy *b) {
                    return a->getTraveledDistance() > b->getTraveledDistance();
                }
              );
}

void SceneGeneric::cleanupEnemies() {
    for (int i = enemies_.size() - 1; i >= 0; --i) {
        Enemy *enemy = enemies_[i];          
        if (enemy->hasReachedEnd()) {
            if (enemy->getHealth() <= 0) {
                goldCnt_ += enemy->getReward();
                emit signalUpdateGold(goldCnt_);
            }
            enemies_.removeAt(i);
            removeItem(enemy);
            delete enemy; // Reward player with gold here
        }
    }
}

void SceneGeneric::updateTowers() {
    auto it = towers_.begin();
    // requests to sell
    while (it != towers_.end()) {
        TowerGeneric *tower = *it;
        if (tower->isToSell()) {
            it = towers_.erase(it);
            removeItem(tower);
            goldCnt_ += tower->getPrice();
            tower->detachTile();
            delete tower;

            emit signalUpdateGold(goldCnt_);
            continue;
        }
        tower->updateCooldown();
        ++it;
    }
    // Tower atack
    for (TowerGeneric *tower : towers_) {
        if (!tower->isReadyToAtack()) {
            continue;
        }
        for (Enemy *enemy : enemies_) {
            if (tower->updateTarget(enemy)) {
                // single target selected
                break;
            }
        }
    }
}

void SceneGeneric::updateProjectiles() {
    // Step all active flying arrows forward and clean up dead ones
    for (int i = projectiles_.size() - 1; i >= 0; --i) {
        Projectile* prj = projectiles_[i];
        prj->advanceFrame();

        if (prj->isDead()) {
            removeItem(prj);
            projectiles_.removeAt(i);
            delete prj;
        }
    }
}

void SceneGeneric::gameLoop() {
    updateEnemies();
    sortEnemies();

    updateTowers();

    updateProjectiles();

    cleanupEnemies();
}
