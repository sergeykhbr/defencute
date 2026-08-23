// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets/QApplication>
#include "GameWindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    GameWindow window;
    window.setWindowTitle("Simple 2D Tower Defense Step 1");
    window.show();

    return app.exec();
}

