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

HexTile::HexTile(QPointF center, qreal zvalue)
    : QGraphicsObject()
{
    registerInterface(static_cast<ITile *>(this));

    brush_ = QBrush(Qt::transparent);
    pen_ = QPen(QColor(240, 240, 240));
    for (int i = 0; i < 6; ++i) {
        qreal angle_rad = M_PI / 3 * i + M_PI / 2;
        polygon_ << QPointF(center.x() + HEX_RADIUS_X * cos(angle_rad),
                           center.y() + HEX_RADIUS_Y * sin(angle_rad));
    }
    shape_.addPolygon(polygon_);
    setZValue(zvalue);

    spriteSheet_.load(":/images/tile.png");
    sprite_ = spriteSheet_.copy(0, 0, 64, 64);

    center_ = center;
    isSelected_ = false;
}

void HexTile::changeState(EState newstate) {
    ITile::changeState(newstate);
    if (newstate == ITile::TileRoute) {
        brush_ = QBrush(QColor(230, 230, 230));
        pen_ = QPen(Qt::NoPen);
        sprite_ = spriteSheet_.copy(64, 0, 64, 64);
    } else {
        brush_ = QBrush(Qt::transparent);
        pen_ = QPen(QColor(240, 240, 240));
    }
}

QRectF HexTile::boundingRect() const {
    return polygon_.boundingRect();
}

void HexTile::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    

    // Draw your texture sprite centered perfectly over the local origin (0,0)
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
#if 0
    // Width = 2 * HEX_RADIUS (48.0), Height = HEX_HEIGHT (~41.5)
    QSize hexSize(48, 41); 
    QPixmap scaledTexture = sprite_.scaled(hexSize,
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation);

    // 2. Wrap the image into a texture brush
    QBrush textureBrush(scaledTexture);

    // 3. Shift the image origin to line up with the top-left of this tile's bounding box
    QTransform brushTransform;
    brushTransform.translate(-24.0, -20.5); // Moves brush handle back matching half-widths
    textureBrush.setTransform(brushTransform);

    // 4. Set the brush and draw the polygon shape! 
    // Qt will automatically clip the image to fit perfectly inside the 6 sides
    painter->setPen(QPen(QColor(220, 220, 220), 1)); // Thin subtle border line
    painter->setBrush(textureBrush);
    painter->drawPolygon(polygon_);
#else
    painter->save();
    painter->setClipPath(shape());
    qreal topLeftX = -(sprite_.width() / 2.0);
    qreal topLeftY = -(sprite_.height() / 2.0);
    painter->drawPixmap(center_.x() + topLeftX, center_.y() + topLeftY, sprite_);
    painter->restore();
#endif

    if (isSelected_ ) {
        QPen shapePen(QColor(0, 200, 255, 255), 1.5, Qt::SolidLine);
        shapePen.setCosmetic(true); 
        painter->save();
        painter->setPen(shapePen);
        painter->setBrush(QColor(0, 200, 255, 30)); // Soft translucent blue fill
        painter->drawPath(shape());
        painter->restore();
    }
}

// Overriding shape prevents mouse hover/clicks 
// outside the literal bounds of the hexagon points
QPainterPath HexTile::shape() const {
    return shape_;
}


