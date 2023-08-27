// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets/QApplication>
#include "test1/Test1GraphicsScene.h"
#include "rogue/RogueWindow.h"
#include "pingpong/PingPongStateMachine.h"
#include "GameGraphicsView.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    PingPongStateMachine pingpongmachine;

    Window rogue;

    Test1GraphicsScene scene;
    GameGraphicsView window(&scene);
    window.resize(300, 300);

    rogue.show();
    window.show();

    return app.exec();
}

