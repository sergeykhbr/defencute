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
#include <QJsonObject>
#include <QPainter>
#include "towermenubtn.h"

class TowerMenu : public QGraphicsObject {
    Q_OBJECT
 public:
    TowerMenu(QGraphicsObject *parent, QJsonObject &cfg);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter,
                       const QStyleOptionGraphicsItem *,
                       QWidget *) override;
 signals:
    void signalMenuRequest(QString &action);

 protected slots:
    void slotBtnPressed(QString &action);

 private:
    TowerMenuButton* upgradeBtn_;
    TowerMenuButton* sellBtn_;
};
