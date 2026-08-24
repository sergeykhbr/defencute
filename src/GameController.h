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
    void goldChanged(int newGold);
    void livesChanged(int newLives);
    void gameOver();

 private slots:
    void tick();

 private:
    // Helper to turn grid columns/rows into exact screen pixel centers
    void gameLoop();

 private:
    QGraphicsView *gameView_;
    SceneGeneric *gameScene_;
    QTimer tmr_;
    int goldCnt_;
    int livesCnt_;
};
