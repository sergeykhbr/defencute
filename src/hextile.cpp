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
    : QGraphicsPolygonItem(),
    tower_(nullptr)
{
    QBrush bgBrush = isPath ? QBrush(QColor(230, 230, 230))
                            : QBrush(Qt::transparent);
    QPen bgPen = isPath ? QPen(Qt::NoPen)
                        : QPen(QColor(240, 240, 240));
    QPolygonF hexLoop;

    for (int i = 0; i < 6; ++i) {
        qreal angle_rad = M_PI / 3 * i;
        hexLoop << QPointF(center.x() + HEX_RADIUS_X * cos(angle_rad),
                           center.y() + HEX_RADIUS_Y * sin(angle_rad));
    }
    setPolygon(hexLoop);
    setPen(bgPen);
    setBrush(bgBrush);
    setZValue(-10);

    isPath_ = isPath;
    center_ = center;
    isBlocked_ = false;
}
