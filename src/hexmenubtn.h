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

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QObject>
#include <QJsonObject>
#include <IScene.h>
#include <ITower.h>

class HexMenuButton : public QGraphicsObject {
    Q_OBJECT
 public:
    HexMenuButton(QGraphicsObject* parent,
                  QJsonObject &cfg);

    QRectF boundingRect() const override;

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem*,
               QWidget*) override;

 signals:
    void signalPressed(QJsonObject &twrcfg);

 protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
 private:
    QJsonObject twrcfg_;
    qreal radius_;
    int price_;
    bool isHovered_;
    QPixmap coverImage_;
    IScene *iscene_;
};
