#pragma once

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QVector2D>
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
    int getHealth() { return health_; }

 public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;

 private:
    QPixmap spriteSheet_;
    QList<QPointF> pathPoints;
    int currentWaypointIndex = 0;
    
    qreal speed = 2.0;
    int currentFrameIndex_; // Cycles: 0, 1, 2
    int animationTimer_;    // Counts game ticks to slow down animation speed
    int directionRow_;      // 0 = Down, 1 = Left, 2 = Right, 3 = Up
    int health_;
    int healthMax_;
};
