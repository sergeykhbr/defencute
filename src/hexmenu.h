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
#include <QPainter>
#include <QJsonObject>
#include <IScene.h>
#include "hexmenubtn.h"

class HexMenu : public QGraphicsObject {
    Q_OBJECT
 public:
    explicit HexMenu(QJsonObject &cfg);
    virtual ~HexMenu();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter,
                       const QStyleOptionGraphicsItem *,
                       QWidget *) override;

 protected slots:
    void slotButtonClicked(QString &towername);

 private:
    QJsonObject &cfg_;
    IScene *iscene_;
    int btnMax_;
    qreal btnRadius_;
    qreal btnOrbit_;
    HexMenuButton** Btn_;
    QRectF boundRect_;
};
