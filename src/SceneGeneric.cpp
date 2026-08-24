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

#include "SceneGeneric.h"

const qreal HEX_RADIUS = 24.0;
const qreal HEX_HEIGHT = std::sqrt(3) * HEX_RADIUS;
const qreal HORIZ_SPACING = 1.5 * HEX_RADIUS;

SceneGeneric::SceneGeneric(QObject *parent) : QGraphicsScene(parent) {
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

            QBrush bgBrush = isPath ? QBrush(QColor(230, 230, 230)) : QBrush(Qt::transparent);
            QPen bgPen = isPath ? QPen(Qt::NoPen) : QPen(QColor(240, 240, 240));

            QPolygonF hexLoop;
            for (int i = 0; i < 6; ++i) {
                qreal angle_rad = M_PI / 3 * i;
                hexLoop << QPointF(center.x() + HEX_RADIUS * cos(angle_rad), center.y() + HEX_RADIUS * sin(angle_rad));
            }
            QGraphicsPolygonItem* bgHex = addPolygon(hexLoop, bgPen, bgBrush);
            bgHex->setData(HexPathRole, isPath);
            bgHex->setData(HexCenter, center);
            bgHex->setData(HexUnavailbleRole, isPath);
            bgHex->setData(HexTowerType, 0);
            bgHex->setZValue(-10);
        }
    }
    // Spawn our path-following circle enemy
    enemy_[0] = new Enemy(visualPathPixelPoints, QPointF(0, 15));
    enemy_[1] = new Enemy(visualPathPixelPoints, QPointF(0, -10));
    addItem(enemy_[0]);
    addItem(enemy_[1]);
}

void SceneGeneric::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseMoveEvent(event);

    // Find the exact item under the cursor
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    QGraphicsPolygonItem *hex = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);

    // Check if it's one of your valid hex tiles
    if (hex && !hex->data(HexPathRole).toBool()) {
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

void SceneGeneric::resetCurrentHighlight() {
    if (currentHoveredHex_) {
        currentHoveredHex_->setBrush(oldBrush_);
        currentHoveredHex_ = nullptr;
    }
}

void SceneGeneric::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QPointF scenePos = event->scenePos();

    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    QGraphicsPolygonItem *hex = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);

    if (!hex) {
        return;
    }
    if (hex->data(HexPathRole).toBool()
        || hex->data(HexUnavailbleRole).toBool()
        || hex->data(HexTowerType).toInt() != 0) {
        return;
    }


    QPointF snapCenter = hex->data(HexCenter).toPointF();


    for (Tower* t : towers) {
        if (std::abs(t->x() - snapCenter.x()) < 5 && std::abs(t->y() - snapCenter.y()) < 5) {
            return; // Already occupied
        }
    }

    Tower* newTower = new Tower(snapCenter, this);
    addItem(newTower);
    towers.append(newTower);
}

// Helper to turn grid columns/rows into exact screen pixel centers
QPointF SceneGeneric::getHexCenter(int col, int row) {
    qreal posX = col * HORIZ_SPACING + HEX_RADIUS;
    qreal posY = row * HEX_HEIGHT + (HEX_HEIGHT / 2.0);
    if (col % 2 != 0) {
        posY += HEX_HEIGHT / 2.0;
    }
    return QPointF(posX, posY);
}

void SceneGeneric::addBallistic(BallisticGeneric *p) {
    listBallistic_.append(p);
    addItem(p);
}

void SceneGeneric::gameLoop() {
    for (int i = 0; i < 2; i++) {
        if (!enemy_[i]) {
            continue;
        }
        enemy_[i]->move();
            
        // Loop or restart enemy if it gets to the final waypoint or dies
        if (enemy_[i]->hasReachedEnd() || enemy_[i]->getHealth() <= 0) {
            enemy_[i]->resetPosition();
        }
    }

    for (Tower* tower : towers) {
        tower->updateCooldown();
    }

    for (Tower* tower : towers) {
        for (int i = 0; i < 2; i++) {
            tower->updateTarget(enemy_[i]);
        }
    }

    // C. Step all active flying arrows forward and clean up dead ones
    for (int i = listBallistic_.size() - 1; i >= 0; --i) {
        BallisticGeneric* arrow = listBallistic_[i];
        arrow->advanceFrame();

        if (arrow->isDead) {
            removeItem(arrow);
            listBallistic_.removeAt(i);
            delete arrow;
        }
    }
}
