#ifndef POISONFROG_H
#define POISONFROG_H

#include "Enemy.h"
#include <vector>
#include <cmath>

class PoisonFrog : public Enemy {
private:
    std::vector<sf::Vector2f> waypoints;
    int currentWaypointIndex = 0;

public:
    PoisonFrog(std::vector<sf::Vector2f> path)
        : Enemy(path.empty() ? sf::Vector2f(0.0f, 0.0f) : path[0], 150.0f, 50.0f),
        waypoints(path)
    {
        // 1. Setup the animation frames
        setupAnimation("frog_sheet.png", 187, 111, 4);

        // 2. Scale the frog down to 35% of its original image size
        enemySprite.setScale(sf::Vector2f(0.25f, 0.25f));
    }

    void update(sf::Time deltaTime) override {
        move(deltaTime);
    }

  
    void move(sf::Time deltaTime) override {
        
        animate(deltaTime);

        // The Movement Math
        if (currentWaypointIndex < waypoints.size()) {

            sf::Vector2f target = waypoints[currentWaypointIndex] + pathOffset;

            float dx = target.x - position.x;
            float dy = target.y - position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= speed * deltaTime.asSeconds()) {
                // Snap to corner and target the next one
                position = target;
                currentWaypointIndex++;
            }
            else {
                // Take a step towards the target
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