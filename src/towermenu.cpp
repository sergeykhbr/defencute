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

#include "towermenu.h"

TowerMenu::TowerMenu(QGraphicsObject *parent,
                     QJsonObject &cfg)
    : QGraphicsObject(parent)
{
    // Orbit button positioning distance out from the tower center point
    qreal orbitDistance = 45.0; 
    qreal buttonRadius = 14.0;

    QJsonObject btncfg;
    btncfg["TowerName"] = cfg["TowerName"];
    btncfg["radius"] = buttonRadius;

    btncfg["action"] = "Upgrade";
    upgradeBtn_ = new TowerMenuButton(this, btncfg);
    upgradeBtn_->setPos(-orbitDistance, 0);
    connect(upgradeBtn_, &TowerMenuButton::signalPressed,
            this, &TowerMenu::slotBtnPressed);

    btncfg["action"] = "Sell";
    sellBtn_ = new TowerMenuButton(this, btncfg);
    sellBtn_->setPos(orbitDistance, 0);
    connect(sellBtn_, &TowerMenuButton::signalPressed,
            this, &TowerMenu::slotBtnPressed);

    int zdepth = cfg["MenuZDepth"].toInt();
    setZValue(zdepth);
    setVisible(false);
}

QRectF TowerMenu::boundingRect() const {
    return QRectF(-70, -70, 140, 140); // Surrounding bounding diameter box
}

void TowerMenu::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *,
                      QWidget *) {
    // Draw a light translucent radial ring guideline showing interaction tracks
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(255, 255, 255, 60), 1, Qt::DashLine));
    painter->setBrush(QColor(0, 0, 0, 20)); // Soft dark circle backdrop
    painter->drawEllipse(QRectF(-45, -45, 90, 90));

#if 0
    // Debug draw boundRect:
    painter->save();
    QPen debugPen(QColor(255, 0, 100, 220), 1.5, Qt::DashLine);
    debugPen.setCosmetic(true); // THE SECRET: Prevents line scaling when zooming the view camera
    painter->setPen(debugPen);
    painter->setBrush(QColor(255, 0, 100, 20)); // Soft translucent red fill back drop highlight
    painter->drawRect(this->boundingRect());
    painter->restore();
#endif
}

void TowerMenu::slotBtnPressed(QString &action) {
    emit signalMenuRequest(action);
}
