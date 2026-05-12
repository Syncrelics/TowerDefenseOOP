#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>


class Map {
private:
    int tileSize;
    std::vector<sf::Vector2f> path; // List of waypoints for enemies to follow
    sf::VertexArray gridLines;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:
   
    Map(int size = 40);

    
    void render(sf::RenderTarget& target) const;

    
    bool isTileBuildable(sf::Vector2f worldPos) const;

    const std::vector<sf::Vector2f>& getPath() const { return path; }
};

#endif