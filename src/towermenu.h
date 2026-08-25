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

#include <QGraphicsItem>
#include <QPainter>
#include "towermenubtn.h"

class TowerMenu : public QGraphicsItem {
public:
    explicit TowerMenu(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {
        // Orbit button positioning distance out from the tower center point
        qreal orbitDistance = 45.0; 
        qreal buttonRadius = 14.0;

        // Position Upgrade button to the Left, Sell button to the Right
        upgradeBtn_ = new TowerMenuButton(TowerMenuButton::Upgrade, buttonRadius, this);
        upgradeBtn_->setPos(-orbitDistance, 0);

        sellBtn_ = new TowerMenuButton(TowerMenuButton::Sell, buttonRadius, this);
        sellBtn_->setPos(orbitDistance, 0);

        setZValue(2000); // Always stay stacked on top of towers and projectiles
        setVisible(false); // Hidden until selected
    }

    QRectF boundingRect() const override {
        return QRectF(-70, -70, 140, 140); // Surrounding bounding diameter box
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        // Draw a light translucent radial ring guideline showing interaction tracks
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(QColor(255, 255, 255, 60), 1, Qt::DashLine));
        painter->setBrush(QColor(0, 0, 0, 20)); // Soft dark circle backdrop
        painter->drawEllipse(QRectF(-45, -45, 90, 90));
    }

private:
    TowerMenuButton* upgradeBtn_;
    TowerMenuButton* sellBtn_;
};
