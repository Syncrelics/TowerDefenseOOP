#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <SFML/Audio.hpp>
#include "Entity.h"
#include "Enemy.h" 
#include "Tower.h"
#include "Map.h"

class Game {
private:
    sf::Music bgMusic;
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;
    float currentVolume = 40.0f;
    sf::Text volumeText;
    sf::Text settingsTitleText;
    sf::RectangleShape btnVolDown;
    sf::Text textVolDown;
    sf::RectangleShape btnVolUp;
    sf::Text textVolUp;
    sf::SoundBuffer cannonBuf;
    sf::SoundBuffer sniperBuf;
    sf::SoundBuffer mgBuf;
    sf::RenderWindow window;
    sf::Clock gameClock;
    sf::Texture baseTexture;
    std::vector<sf::Sprite> bases;
    sf::RectangleShape btnRestart;
    sf::Text textRestart;
    sf::Text gameOverSubText;
    Map gameMap;
    sf::Font uiFont;
    sf::Text goldText;
    sf::Text hudWaveText;
    sf::RectangleShape goldBox; 
    sf::Texture coinTexture;    
    sf::Sprite coinSprite;      

    float coinAnimTimer = 0.0f; 
    int currentCoinFrame = 0;   
    int coinFrameWidth = 0;
    int coinFrameGap = 0;     
    int coinStartX = 0;
    sf::Texture heartTexture;  
    sf::Sprite heartSprite;

    sf::RectangleShape waveBannerBg;
    float waveBannerTimer = 0.0f;
    sf::Text helpText;
    sf::Text backText;
    sf::RectangleShape btnBack;
    
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    // --- GAME STATES ---
    enum class GameState { MENU, HELP, SETTINGS, PLAYING, SHOP, GAMEOVER };
    GameState currentState;

    // --- MENU UI ---
    sf::Text titleText;
    sf::Texture titleTexture;   
    sf::Sprite titleSprite;
    sf::RectangleShape btnNormal;
    sf::RectangleShape btnEndless;
    sf::RectangleShape btnHelp;
    sf::RectangleShape btnMusic;
    sf::Text equippedText;
    float equipMessageTimer = 0.0f;

    void resetGame();
    // --- GAME MODE FLAG ---
    bool isEndlessMode = false;

    
    int gold;
    int lives;
    int waveNumber;
    int selectedTowerType = 1;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    int enemiesSpawnedThisWave = 0;
    int totalEnemiesThisWave = 10; // Start with 10 enemies on Wave 1
    sf::Text waveText;

    // --- GLOBAL TOWER STATS ---
    
    float cannonDamage = 35.0f; int cannonUpCost = 150; int cannonLevel = 0; 
    float sniperDamage = 150.0f; int sniperUpCost = 150; int sniperLevel = 0;
    float mgDamage = 10.0f;     int mgUpCost = 120;     int mgLevel = 0;

    // --- SHOP UI ELEMENTS ---
    sf::Text shopPromptText;
    sf::RectangleShape shopOverlay;
    sf::Text shopTitleText;

    sf::Texture shopCannonTex, shopSniperTex, shopMgTex;
    sf::Sprite shopCannonSpr, shopSniperSpr, shopMgSpr;

    sf::RectangleShape btnUpCannon, btnUpSniper, btnUpMg;
    sf::Text textUpCannon, textUpSniper, textUpMg;
public:
    Game();
    void run(); // The main game loop
};

#endif#pragma once
