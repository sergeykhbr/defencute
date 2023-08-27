#pragma once

#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsWidget>

class MovingWidget : public QGraphicsWidget
{
 public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};
