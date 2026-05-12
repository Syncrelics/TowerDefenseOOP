#ifndef TANKENEMY_H
#define TANKENEMY_H

#include "Enemy.h"
#include <vector>
#include <cmath>

class TankEnemy : public Enemy {
private:
    std::vector<sf::Vector2f> route;
    size_t currentWaypointIndex = 0;

public:
    // HP: 300 (Massive), Speed: 40 (Slow)
    TankEnemy(const std::vector<sf::Vector2f>& mapPath)
        : Enemy(mapPath[0], 300.0f, 40.0f), route(mapPath) {
    }

    void move(sf::Time deltaTime) override {
        if (currentWaypointIndex >= route.size()) return;

        sf::Vector2f target = route[currentWaypointIndex];
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 5.0f) {
            currentWaypointIndex++;
        }
        else {
            sf::Vector2f unitDirection = direction / distance;
            position += unitDirection * speed * deltaTime.asSeconds();
        }
    }

    void update(sf::Time dt) override { move(dt); }

    void render(sf::RenderTarget& target) const override {
        sf::CircleShape body(22.0f); // Massive radius
        body.setOrigin(sf::Vector2f(22.0f, 22.0f));
        body.setPosition(position);
        body.setFillColor(sf::Color::Blue); // Blue for tank
        target.draw(body);

        renderHPBar(target);
    }
};

#endif