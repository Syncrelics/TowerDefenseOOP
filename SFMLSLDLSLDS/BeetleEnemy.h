#ifndef BEETLEENEMY_H
#define BEETLEENEMY_H

#include "Enemy.h"
#include <vector>
#include <cmath>

class BeetleEnemy : public Enemy {
private:
    std::vector<sf::Vector2f> waypoints;
    int currentWaypointIndex = 0;

public:
    BeetleEnemy(std::vector<sf::Vector2f> path)
        : Enemy(path.empty() ? sf::Vector2f(0.0f, 0.0f) : path[0], 600.0f, 25.0f), 
        waypoints(path)
    {
        
        setupAnimation("beetle_sheet.png", 80, 72, 4);

        
        enemySprite.setScale(sf::Vector2f(0.6f, 0.6f));
    }

    void update(sf::Time deltaTime) override {
        move(deltaTime);
    }

    void move(sf::Time deltaTime) override {
        animate(deltaTime);

        if (currentWaypointIndex < waypoints.size()) {
            sf::Vector2f target = waypoints[currentWaypointIndex] + pathOffset;
            float dx = target.x - position.x;
            float dy = target.y - position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= speed * deltaTime.asSeconds()) {
                position = target;
                currentWaypointIndex++;
            }
            else {
                float dirX = dx / distance;
                float dirY = dy / distance;
                position.x += dirX * speed * deltaTime.asSeconds();
                position.y += dirY * speed * deltaTime.asSeconds();
            }

            
            float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
            enemySprite.setRotation(sf::degrees(angle));
        }
    }
};

#endif