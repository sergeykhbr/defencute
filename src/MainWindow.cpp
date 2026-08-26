/*
 *  Copyright 2026 Sergey Khabarov, sergeykhbr@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include "GameView.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_isPaused(false) {
    // 1. Create the master layout container
    QWidget *centralWidget = new QWidget(this);
    m_stackedLayout = new QStackedLayout(centralWidget);
    setCentralWidget(centralWidget);


    GameView *view = new GameView();

    // ==========================================
    // SCREEN 1: THE ESC MENU INTERFACE Overlay
    // ==========================================
    QWidget *escMenuScreen = new QWidget(this);
    QVBoxLayout *menuLayout = new QVBoxLayout(escMenuScreen);
    menuLayout->setAlignment(Qt::AlignCenter); // Centers menu widgets on screen

    QLabel *menuTitle = new QLabel("GAME PAUSED", this);
    menuTitle->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");
    
    QPushButton *resumeBtn = new QPushButton("Resume Game", this);
    QPushButton *restartBtn = new QPushButton("Restart Map", this);
    QPushButton *exitBtn = new QPushButton("Exit to Desktop", this);

    // Enforce matching dimensions for standard button styling consistency
    resumeBtn->setFixedWidth(200);
    restartBtn->setFixedWidth(200);
    exitBtn->setFixedWidth(200);

    menuLayout->addWidget(menuTitle);
    menuLayout->addWidget(resumeBtn);
    menuLayout->addWidget(restartBtn);
    menuLayout->addWidget(exitBtn);

    // Add both pages directly into the screen stack registry
    m_stackedLayout->addWidget(view); // Index 0
    m_stackedLayout->addWidget(escMenuScreen);   // Index 1

    // ==========================================
    // CONTROLLER CONFIGURATION & SIGNALS
    // ==========================================
    m_controller = new GameController(view, this);

    // Wire up Menu Buttons actions
    connect(resumeBtn, &QPushButton::clicked, this, [this]() {
        // Simulates an ESC hit to resume game operations
        QKeyEvent fakeEsc(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        keyPressEvent(&fakeEsc);
    });

    connect(restartBtn, &QPushButton::clicked, this, [this]() {
        m_controller->startNewGame();
        m_isPaused = false;
        m_stackedLayout->setCurrentIndex(0); // Switch right back to game action view
    });

    connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);


    // Fire off initialization setup
    m_controller->startNewGame();
}

// Intercept window engine runtime keystrokes globally
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        m_isPaused = !m_isPaused;
        m_controller->togglePause(m_isPaused);

        if (m_isPaused) {
            // Bring up the ESC Overlay Menu UI Screen
            m_stackedLayout->setCurrentIndex(1);
        } else {
            // Clear view and drop back down into normal canvas render index loop
            m_stackedLayout->setCurrentIndex(0);
        }
    } else {
        // Allow fallback controls routing handler pathways to parse other codes
        QMainWindow::keyPressEvent(event);
    }
}
