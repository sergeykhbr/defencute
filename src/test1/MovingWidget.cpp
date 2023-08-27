#pragma once

#include "MovingWidget.h"

void MovingWidget::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *,
                         QWidget *)
{
    painter->fillRect(rect(), Qt::blue);
}
