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

#include <IFace.h>

class ITile : public Interface {
 public:
    enum EState {
        TileEmpty,
        TileRoute,
        TileOccupied,
        TileUnavailable
    };

    ITile() : Interface("ITile"), tilestate_(TileEmpty) {}

    virtual void changeState(EState newstate) { tilestate_ = newstate; }
    virtual bool isBuildAvailable() { return tilestate_ == TileEmpty; }
    virtual QPointF getCenter() = 0;

 protected:
    EState tilestate_;
};


