#include "GameWindow.h"

const qreal HEX_RADIUS = 24.0;
const qreal HEX_HEIGHT = std::sqrt(3) * HEX_RADIUS;
const qreal HORIZ_SPACING = 1.5 * HEX_RADIUS;

GameWindow::GameWindow() {
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);
    setScene(scene);
    setFixedSize(802, 602);

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

            QBrush bgBrush = isPath ? QBrush(QColor(230, 230, 230)) : QBrush(Qt::NoBrush);
            QPen bgPen = isPath ? QPen(Qt::NoPen) : QPen(QColor(240, 240, 240));

            QPolygonF hexLoop;
            for (int i = 0; i < 6; ++i) {
                qreal angle_rad = M_PI / 3 * i;
                hexLoop << QPointF(center.x() + HEX_RADIUS * cos(angle_rad), center.y() + HEX_RADIUS * sin(angle_rad));
            }
            QGraphicsPolygonItem* bgHex = scene->addPolygon(hexLoop, bgPen, bgBrush);
            bgHex->setData(0, isPath); // Tag it so we know it's a road tile
            bgHex->setZValue(-1);
        }
    }
    // Spawn our path-following circle enemy
    enemy = new Enemy(visualPathPixelPoints);
    scene->addItem(enemy);

    // Game Loop Timer (Approx 60 FPS)
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameWindow::gameLoop);
    gameTimer->start(16); 
}

void GameWindow::mouseMoveEvent(QMouseEvent* event) {
    QPointF scenePos = mapToScene(event->pos());

    QGraphicsView::mouseMoveEvent(event);
}

void GameWindow::mousePressEvent(QMouseEvent* event) {
    QPointF scenePos = mapToScene(event->pos());

    int col = std::round(scenePos.x() / HORIZ_SPACING);
    qreal targetY = scenePos.y();
    if (col % 2 != 0) targetY -= (HEX_HEIGHT / 2.0);
    int row = std::round((targetY - (HEX_HEIGHT / 2.0)) / HEX_HEIGHT);

    QPointF snapCenter = getHexCenter(col, row);

    // Simple bound prevention
    if (col < 0 || col >= 23 || row < 0 || row >= 15) return;

    // Prevent building towers directly on top of our new winding road
    // We do this by checking if the clicked spot hits a road hex
    QGraphicsItem* clickedItem = scene->itemAt(snapCenter, transform());
    if (clickedItem && clickedItem->data(0).toBool() == true) {
        return; // Block building on the track!
    }

    for (Tower* t : towers) {
        if (std::abs(t->x() - snapCenter.x()) < 5 && std::abs(t->y() - snapCenter.y()) < 5) {
            return; // Already occupied
        }
    }

    Tower* newTower = new Tower(snapCenter.x(), snapCenter.y(), HEX_RADIUS);
    scene->addItem(newTower);
    towers.append(newTower);
}

// Helper to turn grid columns/rows into exact screen pixel centers
QPointF GameWindow::getHexCenter(int col, int row) {
    qreal posX = col * HORIZ_SPACING + HEX_RADIUS;
    qreal posY = row * HEX_HEIGHT + (HEX_HEIGHT / 2.0);
    if (col % 2 != 0) {
        posY += HEX_HEIGHT / 2.0;
    }
    return QPointF(posX, posY);
}

void GameWindow::gameLoop() {
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
