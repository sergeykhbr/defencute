#include "GameView.h"


GameView::GameView() {
    setFixedSize(802, 602);

    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    setMouseTracking(true);
    viewport()->setMouseTracking(true);
}
