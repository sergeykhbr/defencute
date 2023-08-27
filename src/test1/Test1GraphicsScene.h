#pragma once

#include <QGraphicsScene>
#include "MovingWidget.h"
#include "Test1StateMachine.h"

class Test1GraphicsScene : public QGraphicsScene {
    Q_OBJECT
 public:
    Test1GraphicsScene(QObject *parent = 0);

 private:
    MovingWidget button1_;
    MovingWidget button2_;
    MovingWidget button3_;
    MovingWidget button4_;

    Test1StateMachine *machine_;
};

