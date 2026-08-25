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

HexMenuButton::HexMenuButton(QString name,
                             qreal radius,
                             QGraphicsObject* parent)
    : QGraphicsObject(parent), name_(name), radius_(radius), isHovered_(false) 
{
    setAcceptHoverEvents(true);
}

QRectF HexMenuButton::boundingRect() const {
    return QRectF(-radius_, -radius_, radius_ * 2, radius_ * 2);
}

void HexMenuButton::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem *,
                          QWidget *) {
    // Change color dynamically on mouse hover
    QColor circleColor = isHovered_ ? QColor(40, 180, 40) : QColor(30, 140, 30); // Green for Upgrade
    //if (type_ == Sell) {
    //    circleColor = isHovered_ ? QColor(220, 60, 60) : QColor(180, 40, 40);   // Red for Sell
    //}

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::white, 2));
    painter->setBrush(circleColor);
    painter->drawEllipse(boundingRect());

    // Draw simple inner action icons (Up Arrow or Dollar Sign)
    painter->setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    if (name_ == "ArrowTower") {
        painter->setFont(QFont("Arial", 10, QFont::Bold));
        painter->drawText(boundingRect(), Qt::AlignCenter, "1");
    } else {
        painter->setFont(QFont("Arial", 10, QFont::Bold));
        painter->drawText(boundingRect(), Qt::AlignCenter, "2");
    }
}

void HexMenuButton::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // Emit custom commands to parent structure
    emit signalPressed(name_);
    event->accept(); // Block event from dropping to map behind
}
