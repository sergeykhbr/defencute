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

#include "common.h"
#include "SceneGeneric.h"
#include <algorithm>

const qreal HEX_HEIGHT = std::sqrt(3) * HEX_RADIUS_Y;
const qreal HORIZ_SPACING = 1.5 * HEX_RADIUS_X;


SceneGeneric::SceneGeneric(QObject *parent)
    : QGraphicsScene(parent),
    hextileSelected_(nullptr)
{
    currentHoveredHex_ = nullptr;
    setSceneRect(0, 0, 800, 600);

    hexHNum_ = static_cast<int>(width() / HORIZ_SPACING) + 1;
    hexVNum_ = static_cast<int>(height() / HEX_HEIGHT) + 1;

    // Define a winding road layout: (Col, Row)
    QList<Waypoint> mapRoad = {
        {0, 2}, {4, 2}, {4, 7}, {10, 7}, {10, 3}, {16, 3}, {16, 10}, {22, 10}
    };

    // Generate the structural path coordinates for our enemy to follow
    for (const auto& wp : mapRoad) {
        visualPathPixelPoints.append(getHexCenter(wp.col, wp.row));
    }

    // Draw the background grid and high-light the path tiles

    for (int col = 0; col < hexHNum_; ++col) {
        for (int row = 0; row < hexVNum_; ++row) {
            QPointF center = getHexCenter(col, row);

            // Determine if this background hex sits on our winding road
            bool isPath = false;
            // Simple logical approximation for drawing the lines connecting waypoints
            for (int i = 0; i < mapRoad.size() - 1; ++i) {
                Waypoint start = mapRoad[i];
                Waypoint end = mapRoad[i+1];
                if (start.col == end.col && col == start.col && row >= std::min(start.row, end.row) && row <= std::max(start.row, end.row)) isPath = true;
                if (start.row == end.row && row == start.row && col >= std::min(start.col, end.col) && col <= std::max(start.col, end.col)) isPath = true;
            }
            addItem(new HexTile(center, isPath));
        }
    }
    hexmenu_ = new HexMenu();
    connect(hexmenu_, &HexMenu::signalBuildTower,
            this, &SceneGeneric::slotBuildTower);

    addItem(hexmenu_);

    // Spawn our path-following circle enemy
    Enemy *enemy = new Enemy(visualPathPixelPoints, QPointF(0, 15));
    addItem(enemy);
    enemies_.push_back(enemy);

    enemy = new Enemy(visualPathPixelPoints, QPointF(0, -10));
    addItem(enemy);
    enemies_.push_back(enemy);
}

void SceneGeneric::resetCurrentHighlight() {
    if (currentHoveredHex_) {
        currentHoveredHex_->setBrush(oldBrush_);
        currentHoveredHex_ = nullptr;
    }
}

void SceneGeneric::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseMoveEvent(event);

    // Find the exact item under the cursor
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    HexTile *hex = qgraphicsitem_cast<HexTile *>(item);

    // Check if it's one of your valid hex tiles
    if (hex && hex->isBuildAvailable()) {
        if (hex != currentHoveredHex_) {
            // Restore the previous hexagon's original color
            resetCurrentHighlight();

            // Save the original brush before overwriting it
            oldBrush_ = hex->brush();
            currentHoveredHex_ = hex;

            // Highlight the new hexagon with Blue
            currentHoveredHex_->setBrush(QBrush(Qt::blue));
        }
    } else {
        // Mouse moved over empty space or a non-hex item
        resetCurrentHighlight();
    }
}

void SceneGeneric::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mousePressEvent(event);

    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());

    closeActiveMenu();

    if (qgraphicsitem_cast<TowerGeneric *>(item)) {
    } else if (qgraphicsitem_cast<HexTile *>(item)) {
        hextileSelected_ = qgraphicsitem_cast<HexTile *>(item);
        if (hextileSelected_->isBuildAvailable()) {
            hexmenu_->setPos(event->scenePos());
            hexmenu_->setVisible(true);
        }
    }
}

void SceneGeneric::closeActiveMenu() {
    hexmenu_->setVisible(false);
}

void SceneGeneric::slotBuildTower(const QString &towerName) {
    TowerGeneric *newTower = nullptr;
    if (towerName == "ArrowTower") {
        newTower = new ArrowTower(hextileSelected_->getCenter(), this);
    } else if (towerName == "GunTower") {
        newTower = new GunTower(hextileSelected_->getCenter(), this);
    }

    if (newTower) {
        addItem(newTower);
        towers_.append(newTower);
        hextileSelected_->attachTower(newTower);
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

void SceneGeneric::gameLoop() {
    // Enemy update positions
    for (Enemy *enemy : enemies_) {
        enemy->move();
    }
    sortEnemies();

    // Tower atack
    for (TowerGeneric *tower : towers_) {
        tower->updateCooldown();
    }
    for (TowerGeneric *tower : towers_) {
        if (!tower->isReadToAtack()) {
            continue;
        }
        for (Enemy *enemy : enemies_) {
            tower->updateTarget(enemy);
        }
    }

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

    // Cleanup dead enemy:
    for (int i = enemies_.size() - 1; i >= 0; --i) {
        Enemy *enemy = enemies_[i];          
        if (enemy->hasReachedEnd() || enemy->getHealth() <= 0) {
            enemy->resetPosition();
        }
#if 0
        if (enemy->hasReachedEnd()) {
            enemies_.removeAt(i);
            removeItem(enemy);
            delete enemy; // Reward player with gold here
        }
#endif
    }
}
