#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QBrush>
#include "tower.h"
#include "enemy.h"

struct Waypoint {
    int col;
    int row;
};

class SceneGeneric : public QGraphicsScene {
 public:
    SceneGeneric(QObject *parent = nullptr);

    virtual void gameLoop();

 protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

 private:
    // Helper to turn grid columns/rows into exact screen pixel centers
    void resetCurrentHighlight();
    QPointF getHexCenter(int col, int row);

 private:
    enum EHexDataRole {
        HexPathRole,
        HexUnavailbleRole,
        HexTowerRole,
        HexTowerType,
        HexTowerLevel
    };
    

    Enemy* enemy;
    QList<Tower*> towers;
    QList<QPointF> visualPathPixelPoints;
    int hexHNum_;       // total HEX per row
    int hexVNum_;       // total HEX per col
    QGraphicsPolygonItem *currentHoveredHex_;
    QBrush oldBrush_;
};
