#pragma once

#include "PingPongGroupStates.h"

PingPongGroupStates::PingPongGroupStates(QStateMachine *machine)
    : QState(QState::ParallelStates),
    pinger_(this),
    ponger_(this) {
    setObjectName("PingPongGroupStates");
    pinger_.setObjectName("pinger");
    ponger_.setObjectName("ponger");

    pinger_.addTransition(new PongTransition);
    ponger_.addTransition(new PingTransition);
}


