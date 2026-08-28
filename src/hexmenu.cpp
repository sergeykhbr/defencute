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

HexMenu::HexMenu(QJsonObject &cfg) : QGraphicsObject(), cfg_(cfg)
{
    ICore * icore = getpCoreInterface();
    QString scenename = cfg["scene"].toString();
    iscene_ = dynamic_cast<IScene *>(icore->getpObjInterface(scenename, "IScene"));

    QJsonObject hexmenu = cfg["HexMenu"].toObject();
    QJsonArray btnTowers = hexmenu["BtnTowers"].toArray();
    QJsonObject towers = cfg["towers"].toObject();

    btnRadius_ = hexmenu["BtnRadius"].toDouble();
    btnOrbit_ = hexmenu["BtnOrbit"].toDouble();
    // Surrounding bounding diameter box
    boundRect_ = QRectF(-(btnRadius_ + btnOrbit_),
                        -(btnRadius_ + btnOrbit_),
                        2*(btnRadius_ + btnOrbit_),
                        2*(btnRadius_ + btnOrbit_));
    btnMax_ = hexmenu["BtnMax"].toInt();
    Btn_ = new HexMenuButton* [btnMax_];

    for (int i = 0; i < btnMax_; i++) {
        QString twrName = btnTowers.at(i).toString();
        if (twrName.isEmpty()) {
            Btn_[i] = nullptr;
            continue;
        }
        Btn_[i] = new HexMenuButton(this, cfg_, btnRadius_);
        Btn_[i]->setPos(btnOrbit_ * sin(i * M_PI/3), 
                        btnOrbit_ * cos(i * M_PI/3));
        Btn_[i]->selectTower(twrName);
        connect(Btn_[i], &HexMenuButton::signalPressed,
                this, &HexMenu::slotButtonClicked);
    }

    QJsonValueRef zdepth = cfg_["ZDepth"];
    int z = zdepth.toObject()["HexMenu"].toInt();
    setZValue(z);
    setVisible(false);
}

HexMenu::~HexMenu() {
    delete [] Btn_;
}

QRectF HexMenu::boundingRect() const {
    return boundRect_; 
}

void HexMenu::paint(QPainter* painter,
                    const QStyleOptionGraphicsItem *,
                    QWidget *) {
    // Draw a light translucent radial ring guideline showing interaction tracks
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(255, 255, 255, 60), 1, Qt::DashLine));
    painter->setBrush(QColor(0, 0, 0, 20)); // Soft dark circle backdrop
    painter->drawEllipse(QRectF(-btnOrbit_,
                                -btnOrbit_,
                                2*btnOrbit_,
                                2*btnOrbit_));
}

void HexMenu::slotButtonClicked(QString &towername) {
    iscene_->buildTower(towername);
}