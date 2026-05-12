#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Projectile {
private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float damage;
    float speed = 700.0f; // Fast bullet speed
    bool active = true;

public:
    Projectile(sf::Vector2f start, sf::Vector2f target, float dmg)
        : position(start), damage(dmg) {

        // Calculate the direction the bullet needs to fly
        sf::Vector2f dir = target - start;
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        // Normalize the vector and multiply by speed
        if (length > 0) {
            velocity = sf::Vector2f((dir.x / length) * speed, (dir.y / length) * speed);
        }
        else {
            velocity = sf::Vector2f(0.f, 0.f);
        }
    }

    void update(sf::Time dt) {
        position += velocity * dt.asSeconds();
    }

    void render(sf::RenderTarget& target) const {
        // Radius is 5.0f
        sf::CircleShape bullet(3.0f);

        
        bullet.setOrigin(sf::Vector2f(3.0f, 3.0f));

        bullet.setPosition(position);
        bullet.setFillColor(sf::Color::Black);
        target.draw(bullet);
    }

    // Getters and control functions
    sf::Vector2f getPosition() const { return position; }
    float getDamage() const { return damage; }
    bool isActive() const { return active; }
    void destroy() { active = false; }
};

#endif