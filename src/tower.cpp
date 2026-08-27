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
    QGraphicsObject(),//QGraphicsPixmapItem(),
    ICoreObject(objname),
    menu_(this)
{
    registerInterface(static_cast<ITower *>(this));

    ICore * icore = getpCoreInterface();
    QString scenename = cfg["scene"].toString();
    iscene_ = dynamic_cast<IScene *>(icore->getpObjInterface(scenename, "IScene"));

    setFlags(QGraphicsItem::ItemIsSelectable);

    qreal posx = cfg["posx"].toDouble();
    qreal posy = cfg["posy"].toDouble();
    hexCenter_ = QPointF(posx, posy);
    setZValue(posy);
    attackCooldown_ = 0;
    rangeIndicator_ = 0;

    spriteSheet_.load(cfg["sprite"].toString());
    singleFrame_ = spriteSheet_.copy(0, 0, 64, 64);
    setPos(posx, posy);
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
        menu_.setVisible(selected);
        update();
    }
    return QGraphicsObject::itemChange(change, value);
}

QRectF TowerGeneric::boundingRect() const {
    qreal w = singleFrame_.width();
    qreal h = singleFrame_.height();
    
    // This defines a bounding box where (0,0) sits at the bottom-center of your image
    qreal topLeftX = -(w / 2.0);
    qreal topLeftY = -h;
    
    // Pad the bottom slightly (+30.0) so your custom green selection ring isn't clipped
    return QRectF(topLeftX, topLeftY, w, h + 30.0);
}

void TowerGeneric::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //QGraphicsPixmapItem::paint(painter, option, widget);

    // Create a copy of the paint style options
    QStyleOptionGraphicsItem customOption(*option);
    customOption.state &= ~QStyle::State_Selected;

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    int spriteShiftY = 24 - 2;
    qreal topLeftX = -(singleFrame_.width() / 2.0);
    qreal topLeftY = -singleFrame_.height() + spriteShiftY;
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

void TowerGeneric::updateTarget(Enemy* enemy) {
    if (attackCooldown_ > 0) {
        return;
    }

    if (!enemy || enemy->getHealth() <= 0) {
        return;
    }

    // Calculate distance to enemy
    QVector2D towerPos(hexCenter_);
    QVector2D enemyPos = enemy->getFuturePos(getFramesToTarget());

    // Shoot if enemy is in range and cooldown is ready
    if (!isInRange(enemyPos)) {
        return;
    }

    QPointF targetPoint(enemyPos.x(), enemyPos.y());

    // Create ballistic projectile and add to world container
    Projectile *p = getpProjectile(hexCenter_, targetPoint, enemy);
    iscene_->addProjectile(p);
    attackCooldown_ = cooldownTime_;
}

//////////////////
// Arrow tower
ArrowTower::ArrowTower(QObject *parent,
                       QString objname,
                       QJsonObject &cfg) 
    : TowerGeneric(parent, objname, cfg) //("ArrowTower", pos, scene, ":/images/archer_tower.png")
{
    frameSpeed_ = 25;
    cooldownTime_ = 60;
    range_ = 140;
    damage_ = 25;

    //QPixmap spriteSheet;
    //spriteSheet.load(":/images/build_icons_64x8.png");
    //icon_ = spriteSheet.copy(0 * 64, 0, 64, 64);
}

Projectile *ArrowTower::getpProjectile(QPointF &start,
                                       QPointF &target,
                                       Enemy* enemy) {
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
                       QJsonObject &cfg)//QPointF pos,  QGraphicsScene* scene)
    : TowerGeneric(parent, objname, cfg) //"RifleTower", pos, scene, ":/images/rifle_tower.png")
{
    frameSpeed_ = 7;
    cooldownTime_ = 75;
    range_ = 150;
    damage_ = 30;
    //QPixmap spriteSheet;
    //spriteSheet.load(":/images/build_icons_64x8.png");
    //icon_ = spriteSheet.copy(1 * 64, 0, 64, 64);
}

Projectile *RifleTower::getpProjectile(QPointF &start,
                                     QPointF &target,
                                     Enemy* enemy) {
    return new Bullet(start,
                     target,
                     enemy,
                     getFramesToTarget(),
                     getDamage());
}
