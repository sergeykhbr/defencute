#include "tower.h"

Tower::Tower(qreal x, qreal y, qreal radius) {
    QPolygonF hexPolygon;
    for (int i = 0; i < 6; ++i) {
        qreal angle_rad = M_PI / 3 * i; 
        hexPolygon << QPointF(radius * cos(angle_rad), radius * sin(angle_rad));
    }
    setPolygon(hexPolygon);
    setBrush(Qt::blue);
    setPos(x, y);
}

void Tower::updateTarget(Enemy* enemy) {
    if (!enemy || enemy->health <= 0) return;

    // Calculate distance to enemy
    QVector2D towerPos(x(), y());
    QVector2D enemyPos(enemy->x(), enemy->y());
    qreal distance = towerPos.distanceToPoint(enemyPos);

    if (attackCooldown > 0) {
        attackCooldown--;
        // Revert enemy color back to normal after flash
        if (attackCooldown == 10 && enemy->health > 30) {
            enemy->currentColor = Qt::red;
            //enemy->setBrush(enemy->currentColor);
        }
    }

    // Shoot if enemy is in range and cooldown is ready
    if (distance <= range && attackCooldown == 0) {
        enemy->takeDamage(15);
        attackCooldown = 20; // Set cooldown frames
    }
}
