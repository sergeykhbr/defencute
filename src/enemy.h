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

class Enemy : public QGraphicsObject {
    Q_OBJECT
 public:
    Q_INVOKABLE Enemy(const QList<QPointF>& points, QPointF startOffset);

    void resetPosition();
    void updateVisualFrame();
    QVector2D getFuturePos(int tick);
    void move();
    bool hasReachedEnd() const;
    void takeDamage(int damage);
    int getHealth() { return health_; }
    qreal getTraveledDistance() const { return traveledDistance_; }

 public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;

 private:
    QPixmap spriteSheet_;
    QPixmap singleFrame_;
    QList<QPointF> pathPoints;
    QPointF startOffset_;
    int currentWaypointIndex = 0;
    
    qreal speed_;
    int currentFrameIndex_; // Cycles: 0, 1, 2
    int animationTimer_;    // Counts game ticks to slow down animation speed
    int directionRow_;      // 0 = Down, 1 = Left, 2 = Right, 3 = Up
    int health_;
    int healthMax_;
    qreal traveledDistance_;
};
