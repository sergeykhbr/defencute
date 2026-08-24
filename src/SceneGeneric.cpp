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
            bgHex->setZValue(-1);
        }
    }
    // Spawn our path-following circle enemy
    enemy = new Enemy(visualPathPixelPoints);
    addItem(enemy);
}

void SceneGeneric::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseMoveEvent(event);

    // 2. Find the exact item under the cursor
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    QGraphicsPolygonItem *hex = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);

    // 3. Check if it's one of your valid hex tiles
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

    int col = std::round(scenePos.x() / HORIZ_SPACING);
    qreal targetY = scenePos.y();
    if (col % 2 != 0) targetY -= (HEX_HEIGHT / 2.0);
    int row = std::round((targetY - (HEX_HEIGHT / 2.0)) / HEX_HEIGHT);

    QPointF snapCenter = getHexCenter(col, row);

    // Simple bound prevention
    if (col < 0 || col >= 23 || row < 0 || row >= 15) return;

    // Prevent building towers directly on top of our new winding road
    // We do this by checking if the clicked spot hits a road hex
    QGraphicsItem *clickedItem = itemAt(event->scenePos(), QTransform());
    if (clickedItem && clickedItem->data(0).toBool() == true) {
        return; // Block building on the track!
    }

    for (Tower* t : towers) {
        if (std::abs(t->x() - snapCenter.x()) < 5 && std::abs(t->y() - snapCenter.y()) < 5) {
            return; // Already occupied
        }
    }

    Tower* newTower = new Tower(snapCenter.x(), snapCenter.y(), HEX_RADIUS);
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


void SceneGeneric::gameLoop() {
    if (enemy) {
        enemy->move();
            
        // Loop or restart enemy if it gets to the final waypoint or dies
        if (enemy->hasReachedEnd() || enemy->health <= 0) {
            enemy->resetPosition();
        }
    }

    if (enemy && enemy->health > 0) {
        for (Tower* tower : towers) {
            tower->updateTarget(enemy);
        }
    }
}
