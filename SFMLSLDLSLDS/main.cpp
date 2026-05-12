#include "Game.h"
#include <iostream>

int main() {
    try {
        // Instantiate the game engine
        Game towerDefense;

        // Start the main game loop
        towerDefense.run();
    }
    catch (const std::exception& e) {
        // Catch any SFML or memory initialization errors
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}