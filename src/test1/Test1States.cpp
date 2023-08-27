#include <QtGui/QPainter>
#include "Test1States.h"

Test1States::Test1States(QStateMachine *machine,
                         Test1StateSwitcher *switcher,
                         QObject *w1,
                         QObject *w2,
                         QObject *w3,
                         QObject *w4)
    : QState(0),
    state1_(this),
    state2_(this),
    state3_(this),
    state4_(this),
    state5_(this),
    state6_(this),
    state7_(this),
    parallelAnimationGroup_(0),
    subGroup1_(&parallelAnimationGroup_),
    subGroup2_(&parallelAnimationGroup_),
    subGroup3_(&parallelAnimationGroup_),
    subGroup4_(&parallelAnimationGroup_)
{
    setObjectName("Test1States");

    state1_.assignProperty(w1, "geometry", QRect(100, 0, 50, 50));
    state1_.assignProperty(w2, "geometry", QRect(150, 0, 50, 50));
    state1_.assignProperty(w3, "geometry", QRect(200, 0, 50, 50));
    state1_.assignProperty(w4, "geometry", QRect(250, 0, 50, 50));

    state2_.assignProperty(w1, "geometry", QRect(250, 100, 50, 50));
    state2_.assignProperty(w2, "geometry", QRect(250, 150, 50, 50));
    state2_.assignProperty(w3, "geometry", QRect(250, 200, 50, 50));
    state2_.assignProperty(w4, "geometry", QRect(250, 250, 50, 50));

    state3_.assignProperty(w1, "geometry", QRect(150, 250, 50, 50));
    state3_.assignProperty(w2, "geometry", QRect(100, 250, 50, 50));
    state3_.assignProperty(w3, "geometry", QRect(50, 250, 50, 50));
    state3_.assignProperty(w4, "geometry", QRect(0, 250, 50, 50));

    state4_.assignProperty(w1, "geometry", QRect(0, 150, 50, 50));
    state4_.assignProperty(w2, "geometry", QRect(0, 100, 50, 50));
    state4_.assignProperty(w3, "geometry", QRect(0, 50, 50, 50));
    state4_.assignProperty(w4, "geometry", QRect(0, 0, 50, 50));

    state5_.assignProperty(w1, "geometry", QRect(100, 100, 50, 50));
    state5_.assignProperty(w2, "geometry", QRect(150, 100, 50, 50));
    state5_.assignProperty(w3, "geometry", QRect(100, 150, 50, 50));
    state5_.assignProperty(w4, "geometry", QRect(150, 150, 50, 50));

    state6_.assignProperty(w1, "geometry", QRect(50, 50, 50, 50));
    state6_.assignProperty(w2, "geometry", QRect(200, 50, 50, 50));
    state6_.assignProperty(w3, "geometry", QRect(50, 200, 50, 50));
    state6_.assignProperty(w4, "geometry", QRect(200, 200, 50, 50));

    state7_.assignProperty(w1, "geometry", QRect(0, 0, 50, 50));
    state7_.assignProperty(w2, "geometry", QRect(250, 0, 50, 50));
    state7_.assignProperty(w3, "geometry", QRect(0, 250, 50, 50));
    state7_.assignProperty(w4, "geometry", QRect(250, 250, 50, 50));

    setInitialState(&state1_);

    auto anim = new QPropertyAnimation(w1, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QEasingCurve::OutElastic);
    //parallelAnimationGroup_.addAnimation(anim);
    subGroup1_.addPause(0);
    subGroup1_.addAnimation(anim);

    anim = new QPropertyAnimation(w2, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QEasingCurve::OutElastic);
    subGroup2_.addPause(100);
    subGroup2_.addAnimation(anim);

    anim = new QPropertyAnimation(w3, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QEasingCurve::OutElastic);
    subGroup3_.addPause(150);
    subGroup3_.addAnimation(anim);

    anim = new QPropertyAnimation(w4, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    subGroup4_.addPause(400);
    subGroup4_.addAnimation(anim);


    switcher->addState(&state1_, &parallelAnimationGroup_);
    switcher->addState(&state2_, &parallelAnimationGroup_);
    switcher->addState(&state3_, &parallelAnimationGroup_);
    switcher->addState(&state4_, &parallelAnimationGroup_);
    switcher->addState(&state5_, &parallelAnimationGroup_);
    switcher->addState(&state6_, &parallelAnimationGroup_);
    switcher->addState(&state7_, &parallelAnimationGroup_);
}

