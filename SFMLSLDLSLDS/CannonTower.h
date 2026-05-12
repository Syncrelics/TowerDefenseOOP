#ifndef CANNONTOWER_H
#define CANNONTOWER_H

#include "Tower.h"
#include <iostream>
#include <cmath>

class CannonTower : public Tower {
private:
    sf::Texture cannonTexture;
    sf::Sprite cannonSprite;

public:
    CannonTower(sf::Vector2f pos)
        : Tower(pos, 100.0f, 200.0f, sf::seconds(1.0f), 100),
        cannonSprite(cannonTexture)
    {
        
        if (!cannonTexture.loadFromFile("cannon.png")) {
            std::cout << "ERROR: Could not load cannon.png!" << std::endl;
        }
        cannonSprite.setTexture(cannonTexture, true);

        
        cannonSprite.setScale(sf::Vector2f(0.45f, 0.45f));

        
        float centerX = cannonTexture.getSize().x / 2.0f;
        float centerY = cannonTexture.getSize().y / 2.0f;
        cannonSprite.setOrigin(sf::Vector2f(centerX, centerY));

        cannonSprite.setPosition(position);
    }

    void attack(std::vector<std::unique_ptr<Enemy>>& enemies, sf::Time deltaTime) override {
        // 1. Find the closest enemy in range to track
        Enemy* target = nullptr;
        float closestDistance = range;

        for (auto& enemy : enemies) {
            if (enemy->isDead()) continue;

            float dx = enemy->getPosition().x - position.x;
            float dy = enemy->getPosition().y - position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= closestDistance) {
                closestDistance = distance;
                target = enemy.get();
            }
        }

        // 2. If an enemy is in range, aim and shoot
        if (target != nullptr) {
            float dx = target->getPosition().x - position.x;
            float dy = target->getPosition().y - position.y;

            
            float distance = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

            
            cannonSprite.setRotation(sf::degrees(angle + 90.0f));

            // Fire if the gun is reloaded
            if (fireTimer >= fireRate) {
                
                float barrelLength = 30.0f; 

                float dirX = dx / distance;
                float dirY = dy / distance;

                // Push the starting point forward by the barrel length
                sf::Vector2f spawnPos(
                    position.x + (dirX * barrelLength),
                    position.y + (dirY * barrelLength)
                );

                // Spawn the bullet at the tip (spawnPos) instead of the center (position)
                projectiles.push_back(Projectile(spawnPos, target->getPosition(), damage));

                fireTimer = sf::Time::Zero;
                shootSound->play();
            }
        }
    }

    void render(sf::RenderTarget& target) const override {
        
        target.draw(cannonSprite);

        
        for (const auto& p : projectiles) {
            p.render(target);
        }
    }
};

#endif