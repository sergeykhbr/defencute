#pragma once

#include <QtStateMachine/QState>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QSequentialAnimationGroup>
#include "Test1StateSwitcher.h"

class Test1States : public QState {
 public:
    Test1States(QStateMachine *machine,
                Test1StateSwitcher *switcher,
                QObject *w1,
                QObject *w2,
                QObject *w3,
                QObject *w4);

 private:
    QState state1_;
    QState state2_;
    QState state3_;
    QState state4_;
    QState state5_;
    QState state6_;
    QState state7_;

    QParallelAnimationGroup parallelAnimationGroup_;
    QSequentialAnimationGroup subGroup1_;
    QSequentialAnimationGroup subGroup2_;
    QSequentialAnimationGroup subGroup3_;
    QSequentialAnimationGroup subGroup4_;
};

