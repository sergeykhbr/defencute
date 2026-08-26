#include "GameController.h"

GameController::GameController(QGraphicsView *view, QObject *parent)
    : QObject(parent),
    gameView_(view),
    gameScene_(nullptr),
    goldCnt_(100),
    livesCnt_(20),
    waveCnt_(20)
{
    // Setup the physics/game loop timer (60 FPS)
    connect(&tmr_, &QTimer::timeout, this, &GameController::tick);
}

void GameController::startNewGame() {
    tmr_.stop();
    goldCnt_ = 100;
    livesCnt_ = 20;

    // Wipe out old scene and clean up memory instantly
    if (gameScene_) {
        gameView_->setScene(nullptr);
        delete gameScene_;
    }

    // Initialize a completely fresh level map
    gameScene_ = new SceneGeneric(this);
    gameView_->setScene(gameScene_);

    connect(this, &GameController::signalGoldChanged,
            gameScene_, &SceneGeneric::slotGoldChanged);

    emit signalGoldChanged(goldCnt_);
    emit signalLivesChanged(livesCnt_);
    emit signalWaveChanged(waveCnt_);
        
    tmr_.start(16); // ~60 ticks per second
}

void GameController::togglePause(bool paused) {
    if (paused) {
        tmr_.stop();
    } else {
        tmr_.start(16);
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

void GameController::slotEnemyKilled(int gold) {
    goldCnt_ += gold;
    emit signalGoldChanged(goldCnt_);
}

void GameController::slotTowerSpent(int gold) {
    goldCnt_ -= gold;
    emit signalGoldChanged(goldCnt_);
}
