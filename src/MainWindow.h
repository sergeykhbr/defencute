#pragma once

#include <QMainWindow>
#include <QStackedLayout>
#include "GameController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
 public:
    explicit MainWindow(QWidget *parent = nullptr);

  protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

 private:
    QStackedLayout *m_stackedLayout;
    GameController *m_controller;
    bool m_isPaused;
};
