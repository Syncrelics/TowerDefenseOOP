#ifndef FLYINGENEMY_H
#define FLYINGENEMY_H

#include "Enemy.h"
#include <vector>
#include <cmath>

class FlyingEnemy : public Enemy {
private:
    sf::Vector2f finalTarget;

public:
    FlyingEnemy(std::vector<sf::Vector2f> path)
        : Enemy(path.empty() ? sf::Vector2f(0.0f, 0.0f) : path.front(), 120.0f, 220.0f) // 80 HP, 120 Speed
    {
        // Set the target to the VERY LAST point on the path
        if (!path.empty()) {
            finalTarget = path.back();
        }
        else {
            finalTarget = sf::Vector2f(800.f, 300.f); // Failsafe target
        }

        // Sprite Math: 1024 / 7 frames = 146 width, 122 height
        setupAnimation("flying_sheet.png", 146, 122, 7);

        // Scale it down so it fits nicely on the screen
        enemySprite.setScale(sf::Vector2f(0.2f, 0.2f));
    }

    void update(sf::Time deltaTime) override {
        move(deltaTime);
    }

    void move(sf::Time deltaTime) override {
        
        animate(deltaTime);

        // Calculate distance straight to the final end zone
        float dx = finalTarget.x - position.x;
        float dy = finalTarget.y - position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        // Keep moving as long as we haven't reached the end
        if (distance > speed * deltaTime.asSeconds()) {
            float dirX = dx / distance;
            float dirY = dy / distance;
            position.x += dirX * speed * deltaTime.asSeconds();
            position.y += dirY * speed * deltaTime.asSeconds();
        }
        else {
            // Snap to the end point so Game.cpp can delete it
            position = finalTarget;
        }

        // Rotation math to point directly at the end zone
        float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
        enemySprite.setRotation(sf::degrees(angle));
    }
};

#endif