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

#include "hexmenubtn.h"
#include <ICore.h>

HexMenuButton::HexMenuButton(QGraphicsObject* parent,
                             QJsonObject &cfg)
    : QGraphicsObject(parent),
    twrcfg_(cfg),
    isHovered_(false) 
{
    radius_ = cfg["radius"].toDouble();
    price_ = cfg["price"].toInt();
    QString iconFile = cfg["iconFile"].toString();
    int iconx = cfg["iconx"].toInt();

    ICore * icore = getpCoreInterface();
    QString scenename = cfg["scene"].toString();
    iscene_ = dynamic_cast<IScene *>(icore->getpObjInterface(scenename, "IScene"));

    QPixmap spriteSheet;
    spriteSheet.load(iconFile);

    int targetDiameter = static_cast<int>(radius_ * 2.0);
    QPixmap coverOriginal = spriteSheet.copy(iconx, 0, 64, 64);
    coverImage_ = coverOriginal.scaled(targetDiameter, targetDiameter, 
                                          Qt::KeepAspectRatio, 
                                          Qt::SmoothTransformation);

    setAcceptHoverEvents(true);
}

QRectF HexMenuButton::boundingRect() const {
    return QRectF(-radius_, -radius_, radius_ * 2, radius_ * 2);
}

void HexMenuButton::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem *,
                          QWidget *) {
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw the sprite centered over the local origin (0, 0)
    qreal topLeftX = -radius_;
    qreal topLeftY = -radius_;
    painter->drawPixmap(topLeftX, topLeftY, coverImage_);

    // Add a visual feedback overlay if hovered (like a subtle dark or light tint)
    if (!iscene_->isGoldAvailable(price_)) {
        painter->setBrush(QColor(255, 255, 255, 140));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(boundingRect());
    } else if (isHovered_) {
        painter->setBrush(QColor(255, 255, 255, 40)); // Translucent white tint overlay
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(boundingRect());
    }
}

void HexMenuButton::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (iscene_->isGoldAvailable(price_)) {
        emit signalPressed(twrcfg_);
    }
    event->accept(); // Block event from dropping to map behind
}

void HexMenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent*) {
    isHovered_ = true;
    update();
}

void HexMenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent*) {
    isHovered_ = false;
    update();
}
