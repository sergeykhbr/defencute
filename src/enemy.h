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
#include <ICoreObject.h>
#include <IScene.h>
#include <IEnemy.h>

class Enemy : public QGraphicsObject,
              public ICoreObject,
              public IEnemy {
    Q_OBJECT
 public:
    Q_INVOKABLE Enemy(QObject *parent,
                      QString objname,
                      QJsonObject &arg);

    // IEnemy
    virtual void takeDamage(int damage) override;
    virtual QVector2D getFuturePos(int tick) override;
    virtual int getHealth() override { return health_; }
    virtual int getReward() override { return reward_; }

    // Common methods
    void resetPosition();
    void updateVisualFrame();
    void move();
    bool hasReachedEnd() const;
    qreal getTraveledDistance() const { return traveledDistance_; }

 public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;

 private:
    void distanceToTick(QVector2D dist, int &tickPerStep, QVector2D &inc);

 private:
    QJsonObject &cfg_;
    QPixmap spriteSheet_;
    QPixmap singleFrame_;
    QList<Waypoint> *route_;
    QVector2D curpos_;
    QVector2D tickDistance2D_;
    QPointF startOffset_;
    int currentWaypointIndex_;
    int tickPerStep_;
    int tickDeadCountdown_;

    IScene *iscene_;

    qreal speed_;
    int currentFrameIndex_; // Cycles: 0, 1, 2
    int animationTimer_;    // Counts game ticks to slow down animation speed
    int directionRow_;      // 0 = Down, 1 = Left, 2 = Right, 3 = Up
    int health_;
    int healthMax_;
    int reward_;
    qreal traveledDistance_;
};
