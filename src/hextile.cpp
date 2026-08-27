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

#include "common.h"
#include "hextile.h"

HexTile::HexTile(QPointF center, bool isPath)
    : QGraphicsObject(),
    itower_(nullptr)
{
    brush_ = isPath ? QBrush(QColor(230, 230, 230))
                            : QBrush(Qt::transparent);
    pen_ = isPath ? QPen(Qt::NoPen)
                        : QPen(QColor(240, 240, 240));
    for (int i = 0; i < 6; ++i) {
        qreal angle_rad = M_PI / 3 * i;
        polygon_ << QPointF(center.x() + HEX_RADIUS_X * cos(angle_rad),
                           center.y() + HEX_RADIUS_Y * sin(angle_rad));
    }
    setZValue(-10);

    isPath_ = isPath;
    center_ = center;
    isBlocked_ = false;
    isSelected_ = false;
}

QRectF HexTile::boundingRect() const {
    return polygon_.boundingRect();
}

void HexTile::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen_);
    if (isSelected_ ) {
        painter->setBrush(QBrush(Qt::blue));
    } else {
        painter->setBrush(brush_);
    }
    painter->drawPolygon(polygon_);}

// Overriding shape prevents mouse hover/clicks 
// outside the literal bounds of the hexagon points
QPainterPath HexTile::shape() const {
    QPainterPath path;
    path.addPolygon(polygon_);
    return path;
}

