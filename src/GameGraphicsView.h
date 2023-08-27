#pragma once

#include <QtWidgets/QGraphicsView>

class GameGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
};
