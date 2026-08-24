// MainWindow.cpp
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

    // ==========================================
    // SCREEN 0: THE LIVE GAMEPLAY CANVAS
    // ==========================================
    QWidget *gameplayScreen = new QWidget(this);
    QVBoxLayout *gameplayLayout = new QVBoxLayout(gameplayScreen);

    // Simple top dashboard bar showing resource metrics
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    QLabel *goldLabel = new QLabel("Gold: 100", this);
    QLabel *livesLabel = new QLabel("Lives: 20", this);
    QLabel *hintLabel = new QLabel("[ Press ESC to Menu ]", this);
    topBarLayout->addWidget(goldLabel);
    topBarLayout->addWidget(livesLabel);
    topBarLayout->addStretch();
    topBarLayout->addWidget(hintLabel);
    gameplayLayout->addLayout(topBarLayout);

    // Main viewport window canvas layout integration
    GameView *view = new GameView();
    gameplayLayout->addWidget(view);

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
    m_stackedLayout->addWidget(gameplayScreen); // Index 0
    m_stackedLayout->addWidget(escMenuScreen);   // Index 1

    // ==========================================
    // CONTROLLER CONFIGURATION & SIGNALS
    // ==========================================
    m_controller = new GameController(view, this);

    // Sync metrics counters seamlessly
    connect(m_controller, &GameController::goldChanged, this, [goldLabel](int g){
        goldLabel->setText(QString("Gold: %1").arg(g));
    });
    connect(m_controller, &GameController::livesChanged, this, [livesLabel](int l){
        livesLabel->setText(QString("Lives: %1").arg(l));
    });

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
