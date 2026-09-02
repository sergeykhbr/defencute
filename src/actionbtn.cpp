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

#include "actionbtn.h"
#include <ICore.h>

ActionButton::ActionButton(QGraphicsObject* parent,
                           QString actionName,
                           QJsonObject &cfg)
    : QGraphicsObject(parent),
    cfg_(cfg),
    actionName_(actionName)
{
    ICore * icore = getpCoreInterface();
    QString scenename = cfg["scene"].toString();
    iscene_ = dynamic_cast<IScene *>(icore->getpObjInterface(scenename, "IScene"));

    QJsonObject actionButtons = cfg["ActionButtons"].toObject();
    QJsonObject btnCfg = actionButtons[actionName_].toObject();
    QString iconFile = btnCfg["iconFile"].toString();
    int iconx = btnCfg["iconx"].toInt();
    int icony = btnCfg["icony"].toInt();
    rect_ = QRect(btnCfg["x"].toInt(),
                  btnCfg["y"].toInt(),
                  btnCfg["width"].toInt(),
                  btnCfg["height"].toInt());

    QPixmap spriteSheet;
    QPixmap coverOriginal;
    spriteSheet.load(iconFile);
    coverOriginal = spriteSheet.copy(iconx, icony, 128, 128);

    coverImage_ = coverOriginal.scaled(rect_.width(),
                                       rect_.height(), 
                                       Qt::KeepAspectRatio, 
                                       Qt::SmoothTransformation);

    QJsonValueRef zdepth = cfg_["ZDepth"];
    int z = zdepth.toObject()["ActionButtons"].toInt();
    setZValue(z);
}

QRectF ActionButton::boundingRect() const {
    return rect_;
}

void ActionButton::paint(QPainter* painter,
                             const QStyleOptionGraphicsItem *,
                             QWidget *) {
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    painter->drawPixmap(rect_, coverImage_);

    if (iscene_->getUserActionTimeout(actionName_)) {
        painter->setBrush(QColor(255, 255, 255, 140));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(boundingRect());
    }
}

void ActionButton::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (iscene_->getUserActionTimeout(actionName_)) {
        event->ignore();
        return;
    }
    emit signalPressed();
    event->accept(); // Block event from dropping to map behind
}
