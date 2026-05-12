#ifndef MACHINEGUNTOWER_H
#define MACHINEGUNTOWER_H

#include "Tower.h"
#include <iostream>
#include <cmath>

class MachineGunTower : public Tower {
private:
    sf::Texture mgTexture;
    sf::Sprite mgSprite;

public:
    MachineGunTower(sf::Vector2f pos)
        : Tower(pos, 15.0f, 150.0f, sf::seconds(0.15f), 120),
        mgSprite(mgTexture) 
    {
        if (!mgTexture.loadFromFile("machine_gun.png")) {
            std::cout << "ERROR: Could not load machine_gun.png!" << std::endl;
        }
        mgSprite.setTexture(mgTexture, true);

        mgSprite.setScale(sf::Vector2f(0.45f, 0.45f));

        float centerX = mgTexture.getSize().x / 2.0f;
        float centerY = mgTexture.getSize().y / 2.0f;
        mgSprite.setOrigin(sf::Vector2f(centerX, centerY));

        mgSprite.setPosition(position);
    }

    void attack(std::vector<std::unique_ptr<Enemy>>& enemies, sf::Time deltaTime) override {
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

        if (target != nullptr) {
            float dx = target->getPosition().x - position.x;
            float dy = target->getPosition().y - position.y;

            float distance = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

            mgSprite.setRotation(sf::degrees(angle + 90.0f));

            if (fireTimer >= fireRate) {
                
                float barrelLength = 25.0f;

                // Find the normalized direction (length of 1)
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
        
        target.draw(mgSprite);

        for (const auto& p : projectiles) {
            p.render(target);
        }
    }
};

#endif