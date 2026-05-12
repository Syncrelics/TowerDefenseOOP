#ifndef TOWER_H
#define TOWER_H

#include "Entity.h"
#include "Enemy.h"
#include "Projectile.h" 
#include <vector>
#include <memory>
#include <optional>
#include <algorithm>    



class Tower : public Entity {
protected:
    float range;   
    float damage = 0.0;       
    sf::Time fireRate;    
    sf::Time fireTimer;   
    int cost;             
    std::optional<sf::Sound> shootSound;
    std::vector<Projectile> projectiles;

public:

    float getDamage() const { return damage; }
    void setDamage(float newDamage) { damage = newDamage; }
    void setShootBuffer(const sf::SoundBuffer& buf) {
        shootSound.emplace(buf);       
        shootSound->setVolume(60.0f);  
    }
    
    Tower(sf::Vector2f pos, float health, float r, sf::Time rate, int c)
        : Entity(pos, health), range(r), fireRate(rate), fireTimer(sf::Time::Zero), cost(c) {
    }

    virtual ~Tower() = default;

    
    virtual void attack(std::vector<std::unique_ptr<Enemy>>& enemies, sf::Time deltaTime) = 0;

   
    void update(sf::Time deltaTime) override {
       
        if (fireTimer < fireRate) {
            fireTimer += deltaTime;
        }
    }

    
    void updateProjectiles(sf::Time deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
        
        for (auto& p : projectiles) {
            if (!p.isActive()) continue;

            p.update(deltaTime);

            // Check distance between this bullet and EVERY enemy
            for (auto& enemy : enemies) {
                if (enemy->isDead()) continue;

                float dx = enemy->getPosition().x - p.getPosition().x;
                float dy = enemy->getPosition().y - p.getPosition().y;
                float distance = std::sqrt(dx * dx + dy * dy);

                
                if (distance <= 30.0f) {
                    enemy->takeDamage(p.getDamage());
                    p.destroy();
                    break;
                }
            }
        }

        // Clean up bullets that exploded
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                [](const Projectile& p) { return !p.isActive(); }),
            projectiles.end()
        );
    }

    
    float getRange() const { return range; }
    int getCost() const { return cost; }

    
    void renderRange(sf::RenderTarget& target) const {
        sf::CircleShape rangeCircle(range);
        rangeCircle.setOrigin(sf::Vector2f(range, range)); 
        rangeCircle.setPosition(position);
        rangeCircle.setFillColor(sf::Color(255, 255, 255, 30)); 
        rangeCircle.setOutlineThickness(1.0f);
        rangeCircle.setOutlineColor(sf::Color::White);
        target.draw(rangeCircle);
    }
};

#endif 