#pragma once

#include <QObject>
#include <QTimer>
#include <QGraphicsView>
#include "SceneGeneric.h"
#include "GameView.h"


class GameController : public QObject {
    Q_OBJECT
 public:
    GameController(QGraphicsView *view, QObject *parent = nullptr);

    void startNewGame();
    void togglePause(bool paused);

 signals:
    void signalGoldChanged(int newGold);
    void signalLivesChanged(int newLives);
    void signalWaveChanged(int newLives);
    void signalGameOver();

 private slots:
    void tick();
    void slotEnemyKilled(int gold);
    void slotTowerSpent(int gold);

 private:
    // Helper to turn grid columns/rows into exact screen pixel centers
    void gameLoop();

 private:
    QGraphicsView *gameView_;
    SceneGeneric *gameScene_;
    QTimer tmr_;
    int goldCnt_;
    int livesCnt_;
    int waveCnt_;
};
