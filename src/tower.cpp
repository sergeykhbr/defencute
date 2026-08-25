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

#include "tower.h"
#include "projectile.h"
#include "SceneGeneric.h"

TowerGeneric::TowerGeneric(QPointF pos,  QGraphicsScene *scene) :
    QGraphicsPixmapItem(),
    scene_(scene),
    hexCenter_(pos),
    menu_(this)
{
    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(pos.y());
    attackCooldown_ = 0;
    rangeIndicator_ = 0;

    spriteSheet_.load(":/images/twr1.png");
    QPixmap singleFrame = spriteSheet_.copy(0, 0, 96, 64);
    setPixmap(singleFrame);

    // THE CORRECTION: Shift the texture offset backward
    // This moves the image top-left corner so that (0,0) sits at the visual center-bottom
    int spriteShiftY = 24 - 2;
    qreal offsetX = -(singleFrame.width() / 2.0);
    qreal offsetY = -singleFrame.height() + spriteShiftY; // Aligns the bottom of the tower sprite to the hex center
    // center position of radius indicator should be shifter realtive picture:
    setOffset(offsetX, offsetY);
    // Position of Tower pixmap
    setPos(pos.x(), pos.y());
}

QVariant TowerGeneric::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemSelectedHasChanged) {
        if (rangeIndicator_ == nullptr) {
            rangeIndicator_ = new QGraphicsEllipseItem(this);
            //  Center the circle around the tower's origin (0, 0 in local space)
            qreal r = getRange();
            rangeIndicator_->setRect(-r, -r, 2 * r, 2* r);
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
    return QGraphicsPixmapItem::itemChange(change, value);
}

void TowerGeneric::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // Create a copy of the paint style options
    QStyleOptionGraphicsItem customOption(*option);
    customOption.state &= ~QStyle::State_Selected;
    QGraphicsPixmapItem::paint(painter, &customOption, widget);

    // Draw your custom Green Selection Circle if the tower is selected
    if (this->isSelected()) {
        painter->setRenderHint(QPainter::Antialiasing);
        
        // Style a crisp green isometric ring flattened slightly for perspective
        painter->setPen(QPen(QColor(250, 155, 50, 200), 2, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        
        // Draw an ellipse right at the base of your tower (around the 0,0 center)
        qreal r = 28.0;
        painter->drawEllipse(QRectF(-r,
                                    -r / 2.0 + 10,
                                    2 * r,
                                    2 * r / 2.0));
    }
}

void TowerGeneric::updateCooldown() {
    if (attackCooldown_ > 0) {
        --attackCooldown_;
    }
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
    qreal distance = towerPos.distanceToPoint(enemyPos);

    // Shoot if enemy is in range and cooldown is ready
    if (distance > getRange()) {
        return;
    }

    QPointF targetPoint(enemyPos.x(), enemyPos.y());

    // Create ballistic projectile and add to world container
    Projectile *p = getpProjectile(hexCenter_, targetPoint, enemy);
    (dynamic_cast<SceneGeneric *>(scene_))->addProjectile(p);
    attackCooldown_ = cooldownTime_;
}

//////////////////
// Arrow tower
ArrowTower::ArrowTower(QPointF pos,  QGraphicsScene* scene) 
    : TowerGeneric(pos, scene)
{
    frameSpeed_ = 25;
    cooldownTime_ = 60;
    range_ = 140;
    damage_ = 25;
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
// Gun tower
GunTower::GunTower(QPointF pos,  QGraphicsScene* scene)
    : TowerGeneric(pos, scene)
{
    frameSpeed_ = 7;
    cooldownTime_ = 75;
    range_ = 150;
    damage_ = 30;
}

Projectile *GunTower::getpProjectile(QPointF &start,
                                     QPointF &target,
                                     Enemy* enemy) {
    return new Bullet(start,
                     target,
                     enemy,
                     getFramesToTarget(),
                     getDamage());
}
