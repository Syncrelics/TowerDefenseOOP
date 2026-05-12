#ifndef SNIPERTOWER_H
#define SNIPERTOWER_H

#include "Tower.h"
#include <iostream>
#include <cmath>

class SniperTower : public Tower {
private:
    sf::Texture towerTexture;
    sf::Sprite towerSprite;

public:
    // pos, damage(250), range(400), fireRate(2.5 seconds), cost(150)
    SniperTower(sf::Vector2f pos)
        : Tower(pos, 250.0f, 400.0f, sf::seconds(2.5f), 150),
        towerSprite(towerTexture)
    {
        fireTimer = fireRate;

        // 1. Load the new sniper texture
        if (!towerTexture.loadFromFile("sniper.png")) {
            std::cout << "ERROR: Could not load sniper.png!" << std::endl;
        }

        // 1.5 Force the sprite to refresh its size
        towerSprite = sf::Sprite(towerTexture);

        // 2. Set the origin to the center
        sf::FloatRect bounds = towerSprite.getLocalBounds();
        towerSprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));

        towerSprite.setPosition(position);

        
        towerSprite.setScale(sf::Vector2f(0.15f, 0.15f));
    }

    void attack(std::vector<std::unique_ptr<Enemy>>& enemies, sf::Time deltaTime) override {
        for (auto& enemy : enemies) {
            if (enemy->isDead()) continue;

            float dx = enemy->getPosition().x - position.x;
            float dy = enemy->getPosition().y - position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            // Huge range check
            if (distance <= range) {

                // Calculate the angle to the target
                float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

                
                towerSprite.setRotation(sf::degrees(angle + 90.0f));

                // Only pull the trigger if the gun is fully reloaded
                if (fireTimer >= fireRate) {
                    
                    float barrelLength = 45.0f; // Longer for sniper rifle

                    // Find the normalized direction (length of 1)
                    float dirX = dx / distance;
                    float dirY = dy / distance;

                    // Push the starting point forward by the barrel length
                    sf::Vector2f spawnPos(
                        position.x + (dirX * barrelLength),
                        position.y + (dirY * barrelLength)
                    );

                    // Spawn the bullet at the tip (spawnPos)
                    projectiles.push_back(Projectile(spawnPos, enemy->getPosition(), damage));

                    fireTimer = sf::Time::Zero;
                    shootSound->play();
                }

                break;
            }
        }
    }

    void render(sf::RenderTarget& target) const override {
        target.draw(towerSprite);

        for (const auto& p : projectiles) {
            p.render(target);
        }
    }
};

#endif