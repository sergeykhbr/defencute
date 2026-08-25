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

#include <QGraphicsPolygonItem>
#include <QPointF>
#include "hexmenu.h"

class HexTile : public QGraphicsPolygonItem {
 public:
    HexTile(QPointF center, bool isPath);

    QPointF getCenter() { return center_; }
    bool isBuildAvailable() {
        return !isPath_ && !isBlocked_ && tower_ == nullptr;
    }
    void attachTower(QGraphicsPixmapItem *tower) { tower_ = tower; }
    void dettachTower() { tower_ = 0; }

 private:
    QGraphicsPixmapItem *tower_;
    QPointF center_;
    bool isPath_;
    bool isBlocked_;
};
