#pragma once

#include <QtCore/QEvent>
#include <QtCore/QEvent>
#include <QtStateMachine/QAbstractTransition>
#include <QtStateMachine/QState>
#include <QtStateMachine/QStateMachine>


class Test1StateSwitcher : public QState {
 public:
    Test1StateSwitcher(QStateMachine *machine);

    void onEntry(QEvent *) override;
    void onExit(QEvent *) override {}

    void addState(QState *state, QAbstractAnimation *animation);

 protected:
    class StateSwitchEvent: public QEvent
    {
     public:
        explicit StateSwitchEvent(int rand) : QEvent(StateSwitchType), rand_(rand) { }

        static constexpr QEvent::Type StateSwitchType = QEvent::Type(QEvent::User + 256);

        int rand() const { return rand_; }

     private:
        int rand_;
    };

    class StateSwitchTransition: public QAbstractTransition
    {
     public:
        explicit StateSwitchTransition(int rand) : QAbstractTransition(), rand_(rand) { }

     protected:
        bool eventTest(QEvent *event) override
        {
            return (event->type() == QEvent::Type(StateSwitchEvent::StateSwitchType))
                && (static_cast<StateSwitchEvent *>(event)->rand() == rand_);
        }

        void onTransition(QEvent *) override {}

     private:
        int rand_;
    };

 private:
    int stateCount_;
    int lastIndex_;
};

