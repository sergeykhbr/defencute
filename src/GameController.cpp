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

#include "GameController.h"

GameController::GameController(QObject *parent,
                               QGraphicsView *view,
                               QJsonObject &cfg)
    : QObject(parent),
    cfg_(cfg),
    gameView_(view),
    gameScene_(nullptr)
{
    icore_ = getpCoreInterface();
    // Setup the physics/game loop timer (60 FPS)
    connect(&tmr_, &QTimer::timeout, this, &GameController::tick);
}

void GameController::startNewGame() {
    tmr_.stop();

    // Wipe out old scene and clean up memory instantly
    if (gameScene_) {
        gameView_->setScene(nullptr);
        delete gameScene_;
    }

    // Initialize a completely fresh level map
    QJsonObject ctrlcfg = cfg_["controller"].toObject();
    gameScene_ = qobject_cast<SceneGeneric *>(
        icore_->createQtClassObject(this,
                                    ctrlcfg["Level"].toString(),
                                    cfg_));
    gameView_->setScene(gameScene_);

    int tickms = ctrlcfg["TickMs"].toInt();
    tmr_.start(tickms); // ~60 ticks per second
}

void GameController::togglePause(bool paused) {
    if (paused) {
        tmr_.stop();
    } else {
        QJsonObject ctrlcfg = cfg_["controller"].toObject();
        int tickms = ctrlcfg["TickMs"].toInt();
        tmr_.start(tickms);
    }
}

void GameController::tick() {
    if (!gameScene_) {
        return;
    }

    // Drive game mechanics detached from the view canvas
    gameScene_->gameLoop();

    // Example interaction: Check if enemy broke your defenses
    /*if (gameScene_->enemy && m_scene->enemy->hasReachedEnd()) {
        m_lives--;
        emit livesChanged(m_lives);
        m_scene->enemy->resetPosition();

        if (m_lives <= 0) {
            tmr_.stop();
            emit gameOver();
        }
    }*/
}
