#pragma once

#include <QtCore/QEvent>

static constexpr QEvent::Type PingEventType = QEvent::Type(QEvent::User + 2);
static constexpr QEvent::Type PongEventType = QEvent::Type(QEvent::User + 3);

class PingEvent : public QEvent
{
public:
    PingEvent() : QEvent(PingEventType) { }
};

class PongEvent : public QEvent
{
public:
    PongEvent() : QEvent(PongEventType) { }
};



