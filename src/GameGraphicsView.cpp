#include "GameGraphicsView.h"

GameGraphicsView::GameGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setFrameStyle(0);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameGraphicsView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene()->sceneRect());
    QGraphicsView::resizeEvent(event);
}
