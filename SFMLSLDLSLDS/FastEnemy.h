#ifndef FASTENEMY_H
#define FASTENEMY_H

#include "Enemy.h"
#include <vector>
#include <cmath>

class FastEnemy : public Enemy {
private:
    std::vector<sf::Vector2f> route;
    size_t currentWaypointIndex = 0;

public:
    // HP: 40 (Low), Speed: 160 (High!)
    FastEnemy(const std::vector<sf::Vector2f>& mapPath)
        : Enemy(mapPath[0], 40.0f, 160.0f), route(mapPath) {
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
        sf::CircleShape body(10.0f); // Smaller radius
        body.setOrigin(sf::Vector2f(10.0f, 10.0f));
        body.setPosition(position);
        body.setFillColor(sf::Color::Yellow); // Yellow for fast
        target.draw(body);

        renderHPBar(target);
    }
};

#endif