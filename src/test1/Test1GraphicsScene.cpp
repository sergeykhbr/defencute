#include "Test1GraphicsScene.h"

Test1GraphicsScene::Test1GraphicsScene(QObject *parent)
    : QGraphicsScene(0, 0, 300, 300) {

    setBackgroundBrush(Qt::black);

    // Higher Z sibling draw on top of lower Z.
    button2_.setZValue(1);
    button3_.setZValue(2);
    button4_.setZValue(3);

    addItem(&button1_);
    addItem(&button2_);
    addItem(&button3_);
    addItem(&button4_);

    machine_ = new Test1StateMachine(this, 
                                    &button1_,
                                    &button2_,
                                    &button3_,
                                    &button4_);
    machine_->start();
}

