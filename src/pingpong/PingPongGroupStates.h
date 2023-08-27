#pragma once

#include <QtCore/QDebug>
#include <QtStateMachine/QAbstractTransition>
#include <QtStateMachine/QState>
#include <QtStateMachine/QStateMachine>
#include "PingPongEvents.h"

class Pinger : public QState
{
public:
    explicit Pinger(QState *parent) : QState(parent) { }

protected:
    void onEntry(QEvent *) override
    {
        machine()->postEvent(new PingEvent);
        qInfo() << "ping?";
    }
};


class PongTransition : public QAbstractTransition
{
public:
    PongTransition() {}

protected:
    bool eventTest(QEvent *e) override { return (e->type() == PingEventType); }

    void onTransition(QEvent *) override
    {
        machine()->postDelayedEvent(new PingEvent, 500);
        qInfo() << "ping?";
    }
};


class PingTransition : public QAbstractTransition
{
public:
    PingTransition() {}

protected:
    bool eventTest(QEvent *e) override { return e->type() == PingEventType; }

    void onTransition(QEvent *) override
    {
        machine()->postDelayedEvent(new PongEvent, 500);
        qInfo() << "pong!";
    }
};



class PingPongGroupStates : public QState {
 public:
    PingPongGroupStates(QStateMachine *machine);

 private:
    Pinger pinger_;
    QState ponger_;
};


