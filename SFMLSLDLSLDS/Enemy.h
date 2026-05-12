#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <iostream>
#include <vector>
#include <string>


class Enemy : public Entity {
protected:
    float speed; 
    sf::Vector2f pathOffset;
    // --- SHIELD VARIABLES ---
    float shield = 0.0f;
    float maxShield = 0.0f;

    // --- ANIMATION & RENDERING VARIABLES ---
    sf::Texture enemyTexture;
    sf::Sprite enemySprite;

    int frameWidth = 0;
    int frameHeight = 0;
    int currentFrame = 0;
    int maxFrames = 1;

    sf::Time animationTimer;
    sf::Time timePerFrame = sf::seconds(0.15f); // Speed of walk/hop animation

public:
    
    Enemy(sf::Vector2f startPos, float health, float movespeed)
        : Entity(startPos, health), speed(movespeed), enemySprite(enemyTexture)
    {
        // Generate a random shift between -15 and +15 pixels
        float offsetX = (rand() % 31) - 15.0f;
        float offsetY = (rand() % 31) - 15.0f;
        pathOffset = sf::Vector2f(offsetX, offsetY);

        // Apply it immediately so they don't all spawn on the exact same pixel
        position += pathOffset;
    }

    virtual ~Enemy() = default;

   
    virtual void move(sf::Time deltaTime) = 0;

    float getSpeed() const { return speed; }

    virtual void takeDamage(float damageAmount) {
        if (shield > 0) {
            shield -= damageAmount;
            if (shield < 0) {
                hp += shield; // Bleed leftover damage into HP
                shield = 0;
            }
        }
        else {
            hp -= damageAmount; // Standard HP damage
        }
    }

    void setupAnimation(const std::string& textureFile, int fWidth, int fHeight, int mFrames) {
        if (!enemyTexture.loadFromFile(textureFile)) {
            std::cout << "ERROR: Could not load " << textureFile << "!\n";
        }
        enemySprite.setTexture(enemyTexture, true);

        frameWidth = fWidth;
        frameHeight = fHeight;
        maxFrames = mFrames;
        currentFrame = 0;

        // Set initial frame window
        enemySprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameWidth, frameHeight }));

        // Center origin so rotation happens around the middle of the sprite
        enemySprite.setOrigin(sf::Vector2f(frameWidth / 2.0f, frameHeight / 2.0f));
        enemySprite.setPosition(position);
    }

    void animate(sf::Time deltaTime) {
        enemySprite.setPosition(position); // Keep sprite attached to physical position
        animationTimer += deltaTime;

        if (animationTimer >= timePerFrame) {
            currentFrame++;
            if (currentFrame >= maxFrames) {
                currentFrame = 0;
            }

            int xOffset = currentFrame * frameWidth;
            enemySprite.setTextureRect(sf::IntRect({ xOffset, 0 }, { frameWidth, frameHeight }));
            animationTimer = sf::Time::Zero;
        }
    }

    virtual void render(sf::RenderTarget& target) const {
        target.draw(enemySprite);
        renderHPBar(target);
    }

    void renderHPBar(sf::RenderTarget& target) const {
        // Hard-coded offset that works well with scaled-down sprites
        float barYOffset = 30.0f;

        // 1. Background (Red - Total possible health)
        sf::RectangleShape backBar({ 40.0f, 5.0f });
        backBar.setFillColor(sf::Color::Red);
        backBar.setPosition(sf::Vector2f(position.x - 20.0f, position.y - barYOffset));

        // 2. Health (Green - Current health)
        sf::RectangleShape frontBar({ 40.0f * (hp / maxHp), 5.0f });
        frontBar.setFillColor(sf::Color::Green);
        frontBar.setPosition(sf::Vector2f(position.x - 20.0f, position.y - barYOffset));

        target.draw(backBar);
        target.draw(frontBar);

        // 3. Shield Bar (Deep Sky Blue) - Stacked directly above health
        if (maxShield > 0 && shield > 0) {
            sf::RectangleShape shieldBar({ 40.0f * (shield / maxShield), 5.0f });
            shieldBar.setFillColor(sf::Color(0, 191, 255));
            shieldBar.setPosition(sf::Vector2f(position.x - 20.0f, position.y - barYOffset - 7.0f));
            target.draw(shieldBar);
        }
    }
};

#endif