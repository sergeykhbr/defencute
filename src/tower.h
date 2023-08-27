#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QPointF>

class Tower : public QObject,
              public QGraphicsPixmapItem {
    Q_OBJECT
 public:
    Tower(QGraphicsItem *parent = 0);
 private:
    QGraphicsPolygonItem * atackArea_;
    QPointF atackDesctination_;
};

