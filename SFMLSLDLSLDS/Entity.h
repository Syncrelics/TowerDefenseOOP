#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>


class Entity {
protected:
    sf::Vector2f position; // The coordinate of the entity on the map ]
    float hp;              // Current health points 
    float maxHp;           // Maximum health for HP bar calculation 

public:
    
    Entity(sf::Vector2f startPos, float health)
        : position(startPos), hp(health), maxHp(health) {
    }

    virtual ~Entity() = default;

    virtual void update(sf::Time deltaTime) = 0;

    virtual void render(sf::RenderTarget& target) const = 0;

    virtual void takeDamage(float amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    // Getters and Setters 
    sf::Vector2f getPosition() const { return position; }
    float getHP() const { return hp; }
    float getMaxHP() const { return maxHp; }

    bool isDead() const { return hp <= 0; }
};

#endif 