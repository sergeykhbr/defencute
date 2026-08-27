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

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QVector2D>
#include <QList>
#include <QJsonObject>
#include <ICoreClass.h>
#include <ICoreObject.h>
#include <IScene.h>
#include <ITower.h>
#include "enemy.h"
#include "projectile.h"
#include "towermenu.h"

class TowerGeneric : public QGraphicsObject,
                     public ICoreObject,
                     public ITower {
    Q_OBJECT
 public:
    TowerGeneric(QObject *parent,
                 QString objname,
                 QJsonObject &cfg);

    // ITower interface
    virtual void updateCooldown();
    virtual bool isReadToAtack() { return attackCooldown_ <= 0; }
    virtual bool isInRange(QVector2D  enemy_pos);
    virtual void updateTarget(Enemy* enemy);
    virtual int getFramesToTarget() { return frameSpeed_; }
    virtual qreal getRange() { return range_; }
    virtual int getDamage() { return damage_; }

 protected:
    // Intercept when selection status shifts dynamically
    virtual QVariant itemChange(GraphicsItemChange change,
                                const QVariant &value) override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual QRectF boundingRect() const override;

    virtual Projectile *getpProjectile(QPointF &start,
                                       QPointF &target,
                                       Enemy* enemy) = 0;

 protected:
    IScene *iscene_;
    QPointF hexCenter_;
    QPixmap spriteSheet_;
    QPixmap singleFrame_;

    int cooldownTime_;
    int attackCooldown_;
    int frameSpeed_;
    qreal range_;
    int damage_;
    QGraphicsEllipseItem *rangeIndicator_;
    TowerMenu menu_;
};

class ArrowTower : public TowerGeneric {
    Q_OBJECT
 public:
    Q_INVOKABLE ArrowTower(QObject *parent,
                            QString objname,
                            QJsonObject &cfg);
 protected:
    virtual Projectile *getpProjectile(QPointF &start,
                                       QPointF &target,
                                       Enemy* enemy);
};

class RifleTower : public TowerGeneric {
    Q_OBJECT
 public:
    Q_INVOKABLE RifleTower(QObject *parent,
                           QString objname,
                           QJsonObject &cfg);
 protected:
    virtual Projectile *getpProjectile(QPointF &start,
                                       QPointF &target,
                                       Enemy* enemy);
};


DECLARE_CLASS(ArrowTower)
DECLARE_CLASS(RifleTower)
