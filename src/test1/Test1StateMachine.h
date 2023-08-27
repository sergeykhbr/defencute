#pragma once

#include <QtCore/QTimer>
#include <QtStateMachine/QState>
#include <QtStateMachine/QStateMachine>
#include "MovingWidget.h"
#include "Test1States.h"
#include "Test1StateSwitcher.h"

class Test1StateMachine : public QStateMachine {
 public:
    Test1StateMachine(QObject *parent,
                        QObject *w1,
                        QObject *w2,
                        QObject *w3,
                        QObject *w4);

 private:
    QTimer timer_;
    Test1StateSwitcher switcher_;
    Test1States groupStates_;
};

