#pragma once

#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QVector2D>
#include "enemy.h"

class Tower : public QGraphicsPolygonItem {
 public:
    Tower(qreal x, qreal y, qreal radius);

    void updateTarget(Enemy* enemy);

 private:
    int attackCooldown = 0;
    int range = 150;
};
