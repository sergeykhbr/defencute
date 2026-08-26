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

#include "hexmenu.h"

static const qreal ORBIT_DISTANCE = 54.0; 
static const qreal BTN_RADIUS = 24.0;


HexMenu::HexMenu(IScene *iscene, QGraphicsObject *parent)
    : QGraphicsObject(parent),
    iscene_(iscene)
{
    // Orbit button positioning distance out from the tower center point

    // Position Upgrade button to the Left, Sell button to the Right
    Btn1_ = new HexMenuButton(iscene, 1, "GunTower", BTN_RADIUS, this);
    Btn1_->setPos(ORBIT_DISTANCE * sin(0), 
                    ORBIT_DISTANCE * cos(0));
    connect(Btn1_, &HexMenuButton::signalPressed,
            this, &HexMenu::slotButtonClicked);


    Btn2_ = new HexMenuButton(iscene, 0, "ArrowTower", BTN_RADIUS, this);
    Btn2_->setPos(ORBIT_DISTANCE * sin(M_PI/3), 
                  ORBIT_DISTANCE * cos(M_PI/3));
    connect(Btn2_, &HexMenuButton::signalPressed,
            this, &HexMenu::slotButtonClicked);

    setZValue(2000);    // Always stay stacked on top of towers and projectiles
    setVisible(false);
}

QRectF HexMenu::boundingRect() const {
    return QRectF(-70, -70, 140, 140); // Surrounding bounding diameter box
}

void HexMenu::paint(QPainter* painter,
                    const QStyleOptionGraphicsItem *,
                    QWidget *) {
    // Draw a light translucent radial ring guideline showing interaction tracks
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(255, 255, 255, 60), 1, Qt::DashLine));
    painter->setBrush(QColor(0, 0, 0, 20)); // Soft dark circle backdrop
    painter->drawEllipse(QRectF(-ORBIT_DISTANCE,
                                -ORBIT_DISTANCE,
                                2*ORBIT_DISTANCE,
                                2*ORBIT_DISTANCE));
}

void HexMenu::slotButtonClicked(const QString &towerName) {
    emit signalBuildTower(towerName);
}