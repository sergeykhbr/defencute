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
#include <ITower.h>

class HexTile : public QGraphicsObject {
    Q_OBJECT
 public:
    HexTile(QPointF center, bool isPath);

    // Mandatory QGraphicsObject overrides replacing internal polygon logic
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override; // Ensures perfect pixel-accurate hexagon mouse clicks

    QPointF getCenter() { return center_; }
    bool isBuildAvailable() {
        return !isPath_ && !isBlocked_ && itower_ == nullptr;
    }
    void attachTower(ITower *tower) { itower_ = tower; }
    void dettachTower() { itower_ = nullptr; }

    void selectOn() { isSelected_ = true; update(); }
    void selectOff() { isSelected_ = false; update(); }

 private:
    ITower *itower_;
    QPointF center_;
    QPolygonF polygon_;
    QBrush brush_;
    QPen pen_;
    bool isSelected_;
    bool isPath_;
    bool isBlocked_;
};
