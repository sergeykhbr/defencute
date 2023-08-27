#pragma once

#include "PingPongStateMachine.h"

PingPongStateMachine::PingPongStateMachine(QObject *parent) :
    QStateMachine(parent),
    group_(this) {
    addState(&group_);
    setInitialState(&group_);
    start();
}


