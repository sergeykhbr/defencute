#include "tower.h"

Tower::Tower(QPointF pos) {
    spriteSheet_.load(":/images/twr1.png");
    QPixmap singleFrame = spriteSheet_.copy(0, 0, 96, 64);
    setPixmap(singleFrame);

    setZValue(pos.y());
    setPos(pos.x() - 96/2, pos.y() - 64/2);
}

void Tower::updateTarget(Enemy* enemy) {
    if (!enemy || enemy->getHealth() <= 0) return;

    // Calculate distance to enemy
    QVector2D towerPos(x(), y());
    QVector2D enemyPos(enemy->x(), enemy->y());
    qreal distance = towerPos.distanceToPoint(enemyPos);

    if (attackCooldown > 0) {
        attackCooldown--;
    }

    // Shoot if enemy is in range and cooldown is ready
    if (distance <= range && attackCooldown == 0) {
        enemy->takeDamage(15);
        attackCooldown = 20; // Set cooldown frames
    }
}
