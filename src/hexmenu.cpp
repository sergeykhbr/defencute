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

#include <ICore.h>
#include <QJsonObject>
#include <QJsonArray>
#include "hexmenu.h"

static const qreal ORBIT_DISTANCE = 54.0; 
static const qreal BTN_RADIUS = 24.0;


HexMenu::HexMenu(QJsonObject &cfg) : QGraphicsObject()
{
    ICore * icore = getpCoreInterface();
    QString scenename = cfg["scene"].toString();
    iscene_ = dynamic_cast<IScene *>(icore->getpObjInterface(scenename, "IScene"));

    QJsonArray jsonBtn;
    QJsonObject btnCfg;
    btnCfg["scene"] = cfg["scene"];
    btnCfg["ClassName"] = "ArrowTower";
    btnCfg["radius"] = BTN_RADIUS;
    btnCfg["price"] = 50;
    btnCfg["iconFile"] = ":/images/build_icons_64x8.png";
    btnCfg["iconx"] = 0;
    jsonBtn.append(btnCfg);

    btnCfg["ClassName"] = "RifleTower";
    btnCfg["iconx"] = 64;
    jsonBtn.append(btnCfg);

    for (int i = 0; i < BTN_MAX; i++ ) {
        QJsonObject obj = jsonBtn.at(i).toObject();
        Btn_[i] = new HexMenuButton(this, obj);
        Btn_[i]->setPos(ORBIT_DISTANCE * sin(i * M_PI/3), 
                        ORBIT_DISTANCE * cos(i * M_PI/3));
        connect(Btn_[i], &HexMenuButton::signalPressed,
                this, &HexMenu::slotButtonClicked);
    }

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

void HexMenu::slotButtonClicked(QString &clsname) {
    iscene_->buildTower(clsname);
}