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

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QVector2D>
#include <cmath>
#include "enemy.h"

class Projectile : public QGraphicsItem {
 public:
    Projectile(QPointF start,
               QPointF target,
               Enemy* enemy,
               qreal speed,     // frames duration
               int damage);

    virtual void advanceFrame();
    virtual bool isDead() { return isDead_; }

 protected:
    virtual void updateTrajectory() = 0;

 protected:
    QPointF startPos_;
    QPointF targetPos_;
    Enemy* targetEnemy_;
    
    qreal progress_;        // Goes from 0.0 (launch) to 1.0 (impact)
    qreal speed_;           // Controls how fast the arrow flies
    int damage_;
    bool isDead_;
};

class Arrow : public Projectile {
 public:
    Arrow(QPointF start,
          QPointF target,
          Enemy* enemy,
          qreal speed,
          int damage);

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual QRectF boundingRect() const override;

 protected:
    virtual void updateTrajectory() override;
 protected:
    qreal maxHeight_;       // Peak altitude of the ballistic arc in pixels
    qreal angleDegrees_;
};

class Bullet : public Projectile {
 public:
    Bullet(QPointF start,
            QPointF target,
            Enemy* enemy,
            qreal speed,
            int damage);

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual QRectF boundingRect() const override;

 protected:
    virtual void updateTrajectory() override;

 protected:
    qreal dx_;
    qreal dy_;
};
