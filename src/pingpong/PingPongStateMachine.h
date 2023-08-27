#pragma once

#include "PingPongGroupStates.h"
#include <QtStateMachine/QStateMachine>

class PingPongStateMachine : public QStateMachine {
 public:
    PingPongStateMachine(QObject *parent = 0);

 private:
    PingPongGroupStates group_;
};

