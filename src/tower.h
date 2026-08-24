#pragma once

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVector2D>
#include "enemy.h"

class Tower : public QGraphicsPixmapItem {
 public:
    Tower(QPointF pos);

    void updateTarget(Enemy* enemy);

 private:
    QPixmap spriteSheet_;
    int attackCooldown = 0;
    int range = 150;
};
