#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QPointF>
#include <QList>
#include "tower.h"
#include "enemy.h"

struct Waypoint {
    int col;
    int row;
};

class GameWindow : public QGraphicsView {
 public:
    GameWindow();

 protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

 private:
    // Helper to turn grid columns/rows into exact screen pixel centers

    QPointF getHexCenter(int col, int row);
    void gameLoop();

 private:
    QGraphicsScene* scene;
    QTimer* gameTimer;
    Enemy* enemy;
    QList<Tower*> towers;
    QList<QPointF> visualPathPixelPoints;
    int hexHNum_;       // total HEX per row
    int hexVNum_;       // total HEX per col
};
