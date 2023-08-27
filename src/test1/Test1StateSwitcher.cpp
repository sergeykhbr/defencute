#pragma once

#include <QtCore/QRandomGenerator>
#include <QtStateMachine/QAbstractTransition>
#include "Test1StateSwitcher.h"

Test1StateSwitcher::Test1StateSwitcher(QStateMachine *machine)
    : QState(machine) {
    stateCount_ = 0;
    lastIndex_ = 0;
    setObjectName("Test1StateSwitcher");
}

void Test1StateSwitcher::onEntry(QEvent *)
{
    int n;
    while ((n = QRandomGenerator::global()->bounded(stateCount_) + 1) == lastIndex_)
    { }
    lastIndex_ = n;
    machine()->postEvent(new StateSwitchEvent(n));
}

void Test1StateSwitcher::addState(QState *state, QAbstractAnimation *animation) {
    auto trans = new StateSwitchTransition(++stateCount_);
    trans->setTargetState(state);
    addTransition(trans);
    trans->addAnimation(animation);
}
