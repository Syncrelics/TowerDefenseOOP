#include "Map.h"
#include <iostream> 
#include <cmath>

// --- CONSTRUCTOR ---
Map::Map(int size) : tileSize(size), bgSprite(bgTexture) {
    
    path.push_back(sf::Vector2f(3.0f, 330.f));   // Start left
    path.push_back(sf::Vector2f(128.f, 334.f)); // First turn (UP)
    path.push_back(sf::Vector2f(126.f, 150.f)); // Second turn (RIGHT)
    path.push_back(sf::Vector2f(288.f, 149.f)); // Third turn (DOWN)
    path.push_back(sf::Vector2f(289.f, 389.f)); // Fourth turn (RIGHT)
    path.push_back(sf::Vector2f(500.f, 387.f)); // Fifth turn (UP)
    path.push_back(sf::Vector2f(506.f, 277.f)); // Sixth turn (RIGHT)
    path.push_back(sf::Vector2f(785.f, 272.f));

    // 2. Load the Background Image
    if (!bgTexture.loadFromFile("grass_bg.png")) {
        std::cout << "ERROR: Could not load grass_bg.png!" << std::endl;
    }

    // Force the sprite to update its size based on the loaded image
    bgSprite.setTexture(bgTexture, true);
    float scaleX = 800.0f / bgTexture.getSize().x;
    float scaleY = 600.0f / bgTexture.getSize().y;
    bgSprite.setScale(sf::Vector2f(scaleX, scaleY));
}

// --- RENDER ---
void Map::render(sf::RenderTarget& target) const {
    
    target.draw(bgSprite);
}

// --- COLLISION LOGIC ---
bool Map::isTileBuildable(sf::Vector2f worldPos) const {
    // Check if the clicked position intersects any part of the invisible path
    for (size_t i = 0; i < path.size() - 1; ++i) {
        sf::Vector2f start = path[i];
        sf::Vector2f end = path[i + 1];

        // Define the bounds of this specific path segment
        float minX = std::min(start.x, end.x);
        float maxX = std::max(start.x, end.x);
        float minY = std::min(start.y, end.y);
        float maxY = std::max(start.y, end.y);

        // Expand the bounds to account for the thickness of the path (tileSize)
        if (start.y == end.y) { // Horizontal path
            minY -= (float)tileSize / 2.0f;
            maxY += (float)tileSize / 2.0f;
            minX -= (float)tileSize / 2.0f;
            maxX += (float)tileSize / 2.0f;
        }
        else if (start.x == end.x) { // Vertical path
            minX -= (float)tileSize / 2.0f;
            maxX += (float)tileSize / 2.0f;
            minY -= (float)tileSize / 2.0f;
            maxY += (float)tileSize / 2.0f;
        }

        // Check if the mouse click is inside this path segment's bounding box
        if (worldPos.x >= minX && worldPos.x <= maxX &&
            worldPos.y >= minY && worldPos.y <= maxY) {
            return false;
        }
    }

    return true; 
}