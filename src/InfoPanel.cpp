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

#include "InfoPanel.h"

InfoPanel::InfoPanel(IScene *iscene, QGraphicsObject* parent)
    : QGraphicsObject(parent),
    iscene_(iscene),
    gold_(0),
    lives_(0),
    wave_(1) 
{
    // 1. Ensure the UI element stays frozen in place relative to the viewport window camera
    setFlag(QGraphicsItem::ItemIgnoresTransformations, false); 
    setZValue(99999); // Force HUD stack overlay above towers and enemies

    // 2. Load and scale your decorative asset background image box
    QPixmap rawBg(":/images/info_panel_bkg.png"); 
    backgroundBg_ = rawBg.scaled(static_cast<int>(width_), static_cast<int>(height_),
                                  Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QRectF InfoPanel::boundingRect() const {
    // Top-left anchored panel allocation spacing frame
    return QRectF(0, 0, width_, height_);
}

void InfoPanel::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    // 1. Draw the clean graphical background board frame texture
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(0, 0, backgroundBg_);

    // 2. Configure stylistic text fonts (matching a bold fantasy theme)
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen(QColor(255, 230, 150)); // Bright golden text color
    painter->setFont(QFont("Arial", 11, QFont::Bold));

    // 3. Render content stats string strings onto calculated coordinate positions
    // Format: Gold on the left, Lives in the middle, Wave on the right
    QString goldText  = QString("%1").arg(gold_);
    QString livesText = QString("%1").arg(lives_);
    QString waveText  = QString("%1").arg(wave_);

    // Draw strings with pixel spacing padding alignments
    painter->drawText(QRectF(39, 10, 70, height_), Qt::AlignLeft, goldText);
    
    painter->setPen(QColor(255, 100, 100)); // Shift color to red for health stats
    painter->drawText(QRectF(120, 10, 60, height_), Qt::AlignLeft, livesText);
    
    painter->setPen(QColor(255, 255, 255)); // White color code text for wave status
    painter->drawText(QRectF(186, 10, 75, height_), Qt::AlignLeft, waveText);
}

void InfoPanel::slotUpdateGold(int gold) {
    gold_ = gold;
    update();
}

void InfoPanel::slotUpdateLives(int lives) {
    lives_ = lives;
    update();
}

void InfoPanel::slotUpdateWave(int wave) {
    wave_ = wave;
    update();
}
