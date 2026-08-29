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

#include <common.h>
#include <ICore.h>
#include "tower.h"
#include "projectile.h"
#include "SceneGeneric.h"

TowerGeneric::TowerGeneric(QObject *parent,
                 QString objname,
                 QJsonObject &cfg) :
    QGraphicsObject(),
    ICoreObject(objname),
    menu_(nullptr)
{
    registerInterface(static_cast<ITower *>(this));

    ICore * icore = getpCoreInterface();
    QString scenename = cfg["scene"].toString();
    iscene_ = dynamic_cast<IScene *>(icore->getpObjInterface(scenename, "IScene"));

    setFlags(QGraphicsItem::ItemIsSelectable);

    qreal posx = cfg["posx"].toDouble();
    qreal posy = cfg["posy"].toDouble();
    hexCenter_ = QPointF(posx, posy);
    spriteShiftY_ = 24 - 2;

    menucfg_["TowerName"] = objname;
    menucfg_["MenuZDepth"] = cfg["MenuZDepth"].toInt();
    menu_ = new TowerMenu(this, menucfg_);
    connect(menu_, &TowerMenu::signalMenuRequest,
            this, &TowerGeneric::slotMenuRequest);

    attackCooldown_ = 0;
    rangeIndicator_ = 0;
    markToSell_ = false;
    price_ = cfg["price"].toInt();

    spriteSheet_.load(cfg["sprite"].toString());
    singleFrame_ = spriteSheet_.copy(0, 0, 64, 64);
    setPos(posx, posy);
    setZValue(posy);
}

QVariant TowerGeneric::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemSelectedHasChanged) {
        if (rangeIndicator_ == nullptr) {
            rangeIndicator_ = new QGraphicsEllipseItem(this);
            //  Center the circle around the tower's origin (0, 0 in local space)
            qreal rx = getRange();
            qreal ry = HEX_X_TO_Y * rx;
            rangeIndicator_->setRect(-rx, -ry, 2 * rx, 2 * ry);
            rangeIndicator_->setBrush(QBrush(QColor(0, 255, 70, 20)));
            rangeIndicator_->setPen(QPen(QColor(0, 255, 70, 150), 2, Qt::SolidLine));
            rangeIndicator_->setVisible(false);
            // Ensure it renders below the tower image but above the map grid
            rangeIndicator_->setZValue(-1); 
        }
        // Automatically toggle visibility based on selection state
        bool selected = value.toBool();
        rangeIndicator_->setVisible(selected);
        menu_->setVisible(selected);
        update();
    }
    return QGraphicsObject::itemChange(change, value);
}

QRectF TowerGeneric::boundingRect() const {
    qreal w = singleFrame_.width();
    qreal h = singleFrame_.height();
    
    // This defines a bounding box where (0,0) sits at the bottom-center of your image
    qreal topLeftX = -(w / 2.0);
    qreal topLeftY = -h + spriteShiftY_;
    
    return QRectF(topLeftX, topLeftY, w, h);
}

void TowerGeneric::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QStyleOptionGraphicsItem customOption(*option);
    customOption.state &= ~QStyle::State_Selected;

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    qreal topLeftX = -(singleFrame_.width() / 2.0);
    qreal topLeftY = -singleFrame_.height() + spriteShiftY_;
    painter->drawPixmap(topLeftX, topLeftY, singleFrame_);

    // Draw your custom Green Selection Circle if the tower is selected
    if (this->isSelected()) {
        painter->setRenderHint(QPainter::Antialiasing);
        
        // Style a crisp green isometric ring flattened slightly for perspective
        painter->setPen(QPen(QColor(250, 155, 50, 200), 2, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        
        // Draw an ellipse right at the base of your tower (around the 0,0 center)
        qreal rx = HEX_RADIUS_X + 4;
        qreal ry = HEX_RADIUS_Y + 2;
        painter->drawEllipse(QRectF(-rx,
                                    -ry / 2.0 + (HEX_RADIUS_Y/2 - 2),
                                    2 * rx,
                                    2 * ry / 2.0));
    }

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

void TowerGeneric::updateCooldown() {
    if (attackCooldown_ > 0) {
        --attackCooldown_;
    }
}

bool TowerGeneric::isInRange(QVector2D enemy_pos) {
    QVector2D diffPos(hexCenter_);
    qreal rx = getRange();
    qreal ry = HEX_X_TO_Y * rx;
    diffPos -= enemy_pos;


    qreal distance = diffPos.x() * diffPos.x() / (rx * rx)
                   + diffPos.y() * diffPos.y() / (ry * ry);
    if (distance <= 1.0) {
        return true;
    }
    return false;
}

void TowerGeneric::updateTarget(IEnemy* ienemy) {
    if (attackCooldown_ > 0) {
        return;
    }

    if (!ienemy || ienemy->getHealth() <= 0) {
        return;
    }

    // Calculate distance to enemy
    QVector2D towerPos(hexCenter_);
    QVector2D enemyPos = ienemy->getFuturePos(getFramesToTarget());

    // Shoot if enemy is in range and cooldown is ready
    if (!isInRange(enemyPos)) {
        return;
    }

    QPointF targetPoint(enemyPos.x(), enemyPos.y());

    // Create ballistic projectile and add to world container
    Projectile *p = createProjectile(hexCenter_, targetPoint, ienemy);
    iscene_->addProjectile(p);
    attackCooldown_ = cooldownTime_;
}

void TowerGeneric::slotMenuRequest(QString &action) {
    qDebug() << getObjName() << action;
    if (action == "Sell") {
        markToSell_ = true;
    }
}

//////////////////
// Arrow tower
ArrowTower::ArrowTower(QObject *parent,
                       QString objname,
                       QJsonObject &cfg) 
    : TowerGeneric(parent, objname, cfg)
{
    frameSpeed_ = 25;
    cooldownTime_ = 60;
    range_ = 140;
    damage_ = 25;
}

Projectile *ArrowTower::createProjectile(QPointF &start,
                                         QPointF &target,
                                         IEnemy* enemy) {
    return new Arrow(start,
                     target,
                     enemy,
                     getFramesToTarget(),
                     getDamage());
}


////////////////
// Rifle tower
RifleTower::RifleTower(QObject *parent,
                       QString objname,
                       QJsonObject &cfg)
    : TowerGeneric(parent, objname, cfg)
{
    frameSpeed_ = 7;
    cooldownTime_ = 75;
    range_ = 150;
    damage_ = 30;
}

Projectile *RifleTower::createProjectile(QPointF &start,
                                     QPointF &target,
                                     IEnemy* enemy) {
    return new Bullet(start,
                     target,
                     enemy,
                     getFramesToTarget(),
                     getDamage());
}
