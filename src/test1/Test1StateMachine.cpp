#include "Test1StateMachine.h"

Test1StateMachine::Test1StateMachine(QObject *parent,
                                     QObject *w1,
                                     QObject *w2,
                                     QObject *w3,
                                     QObject *w4)
    : QStateMachine(parent),
    timer_(this),
    switcher_(this),
    groupStates_(this, &switcher_, w1, w2, w3, w4) {

    timer_.setInterval(1250);
    timer_.setSingleShot(true);

    connect(&groupStates_, SIGNAL(entered()),
            &timer_, SLOT(start()));

    groupStates_.addTransition(&timer_, &QTimer::timeout, &switcher_);

    addState(&groupStates_);
    setInitialState(&groupStates_);
}

