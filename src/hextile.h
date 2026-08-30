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
#include <QGraphicsObject>
#include <QPointF>
#include <QPolygonF>
#include <QBrush>
#include <QPen>
#include "hexmenu.h"
#include <ICoreObject.h>
#include <ITile.h>

class HexTile : public QGraphicsObject,
                public ICoreObject,
                public ITile {
    Q_OBJECT
 public:
    HexTile(QPointF center, qreal zvalue);

    // Mandatory QGraphicsObject overrides replacing internal polygon logic
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QPainterPath shape() const override; // Ensures perfect pixel-accurate hexagon mouse clicks

    // ITile
    virtual void changeState(EState newstate) override;
    virtual QPointF getCenter() override { return center_; }

    // Common public methods:

    void selectOn() { isSelected_ = true; update(); }
    void selectOff() { isSelected_ = false; update(); }

 private:
    QPointF center_;
    QPolygonF polygon_;
    QBrush brush_;
    QPen pen_;
    bool isSelected_;
};
