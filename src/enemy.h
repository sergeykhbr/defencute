#pragma once

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVector2D>
#include <QPolygonF>
#include <QPointF>
#include <QList>
#include <QPixmap>
#include <cmath>

class Enemy : public QGraphicsPixmapItem {
 public:
    Enemy(const QList<QPointF>& points);

    void resetPosition();
    void updateVisualFrame();
    void move();
    bool hasReachedEnd() const;
    void takeDamage(int damage);

 public:
    int health = 100;
    QColor currentColor = Qt::red;

 private:
    QPixmap spriteSheet_;
    QList<QPointF> pathPoints;
    int currentWaypointIndex = 0;
    
    qreal speed = 2.0;
    int currentFrameIndex_; // Cycles: 0, 1, 2
    int animationTimer_;    // Counts game ticks to slow down animation speed
    int directionRow_;      // 0 = Down, 1 = Left, 2 = Right, 3 = Up
};
