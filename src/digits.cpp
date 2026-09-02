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

#include "common.h"
#include "digits.h"
#include <QPixmap>

Digits::Digits(QObject *parent, int x, int y, int width, int height)
    : QObject(parent)
{
    spriteDigits_.load(":/images/digits_gold64.png");
    for (int i = 0; i < 10; i++) {
        digits64_[i] = spriteDigits_.copy(64*i, 0, 64, 64);
        digits_[i] = digits64_[i].scaled(width,
                                        height,
                                        Qt::KeepAspectRatio, 
                                        Qt::SmoothTransformation);
    }
}

void Digits::draw(QPainter *painter, int value) {
}
