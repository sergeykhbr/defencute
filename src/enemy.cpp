#pragma once

#include "enemy.h"

const int FRAME_WIDTH = 96;  // Size of one frame box
const int FRAME_HEIGHT = 64;
const int ANIM_SPEED_TICKS = 8; // Change sprite frame every 8 game loop ticks (~60fps / 8)

Enemy::Enemy(const QList<QPointF>& points) : QGraphicsPixmapItem() {
    pathPoints = points;
    spriteSheet_.load(":/images/base_walk_strip8.png");

    setOffset(-FRAME_WIDTH / 2, -FRAME_HEIGHT / 2);

    //setBrush(currentColor);
    resetPosition();
}

void Enemy::resetPosition() {
    currentWaypointIndex = 0;
    health = 100;
    currentFrameIndex_ = 0;
    animationTimer_ = 0;
    directionRow_ = 0;
    if (!pathPoints.isEmpty()) {
        setPos(pathPoints[0]);
    }
    updateVisualFrame();
}

void Enemy::updateVisualFrame() {
    if (spriteSheet_.isNull()) {
        return;
    }

    // Math out source pixels inside the image asset grid
    int srcX = currentFrameIndex_ * FRAME_WIDTH;
    int srcY = directionRow_ * FRAME_HEIGHT;

    // Crop the single active frame out of the master image sheet
    QPixmap singleFrame = spriteSheet_.copy(srcX, srcY, FRAME_WIDTH, FRAME_HEIGHT);
        
    // Push the cropped picture to Qt's rendering system
    setPixmap(singleFrame);
}

void Enemy::move() {
    if (currentWaypointIndex >= pathPoints.size()) return;

    QPointF target = pathPoints[currentWaypointIndex];
    QVector2D currentPos(x(), y());
    QVector2D targetPos(target.x(), target.y());
        
    // Calculate vector direction and distance to the next hex center
    QVector2D direction = targetPos - currentPos;
    qreal distance = direction.length();

    if (distance <= speed) {
        // Close enough! Snap to target and aim for the next waypoint
        setPos(target);
        currentWaypointIndex++;
    } else {
        // Step forward along the direction vector
        direction.normalize();
        moveBy(direction.x() * speed, direction.y() * speed);
    }

    animationTimer_++;
    if (animationTimer_ >= ANIM_SPEED_TICKS) {
        animationTimer_ = 0;
        currentFrameIndex_ = (currentFrameIndex_ + 1) % 3; // Loop between frames 0, 1, 2
        updateVisualFrame();
    }
}

bool Enemy::hasReachedEnd() const {
    return currentWaypointIndex >= pathPoints.size();
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    // Visual indicator: flash yellow when damaged, turn dark red when low
    if (health <= 30) {
        currentColor = QColor(139, 0, 0); // Dark Red
    } else {
        currentColor = Qt::yellow; 
    }
    //setBrush(currentColor);
}
