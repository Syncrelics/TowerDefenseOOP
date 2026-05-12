#include "Game.h"
#include "BasicEnemy.h"
#include "FastEnemy.h"
#include "TankEnemy.h"
#include "CannonTower.h"
#include "FlyingEnemy.h"
#include "SniperTower.h"
#include "MachineGunTower.h"
#include "PoisonFrog.h"
#include "TribalWarrior.h"
#include "BeetleEnemy.h"
#include <iostream>
#include <cstdlib> 
#include <cstdint> 

Game::Game()
    : window(sf::VideoMode({ 800, 600 }), "Tower Defense"),
    gold(500),
    lives(3),
    waveNumber(1),
    enemiesSpawnedThisWave(0),
    totalEnemiesThisWave(10),
    goldText(uiFont),
    titleText(uiFont),
    waveText(uiFont),
    hudWaveText(uiFont),
    equippedText(uiFont),
    helpText(uiFont),
    backText(uiFont),
    shopPromptText(uiFont),
    shopTitleText(uiFont),
    textUpCannon(uiFont), textUpSniper(uiFont), textUpMg(uiFont),
    volumeText(uiFont),
    settingsTitleText(uiFont),
    textVolDown(uiFont),
    textVolUp(uiFont),
    textRestart(uiFont),            
    gameOverSubText(uiFont),        
    titleSprite(titleTexture),
    coinSprite(coinTexture),
    heartSprite(heartTexture),
    shopCannonSpr(shopCannonTex), shopSniperSpr(shopSniperTex), shopMgSpr(shopMgTex),
    clickSound(clickBuffer),
    currentVolume(40.0f),
    currentState(GameState::MENU),
    isEndlessMode(false)
{
    // ==========================================
    // SETUP FONTS & AUDIO
    // ==========================================
    if (!uiFont.openFromFile("opensans.ttf")) {
        std::cout << "ERROR: Could not load opensans.ttf!" << std::endl;
    }

    if (!bgMusic.openFromFile("theme.ogg")) {
        std::cout << "WARNING: Could not load theme.ogg!" << std::endl;
    }
    else {
        bgMusic.setLooping(true);
        bgMusic.setVolume(currentVolume);
        bgMusic.play();
    }

    if (!clickBuffer.loadFromFile("click.wav")) {
        std::cout << "WARNING: Could not load click.wav!" << std::endl;
    }
    else {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(80.0f);
    }

    if (!cannonBuf.loadFromFile("cannon_shoot.wav")) std::cout << "Missing cannon_shoot.wav\n";
    if (!sniperBuf.loadFromFile("sniper_shoot.wav")) std::cout << "Missing sniper_shoot.wav\n";
    if (!mgBuf.loadFromFile("mg_shoot.wav")) std::cout << "Missing mg_shoot.wav\n";

    equippedText.setCharacterSize(28);
    equippedText.setFillColor(sf::Color(255, 255, 255, 0));
    equippedText.setOutlineThickness(2.0f);
    equippedText.setOutlineColor(sf::Color(0, 0, 0, 0));

    // ==========================================
    // MENU & HELP SCREENS
    // ==========================================
    if (!titleTexture.loadFromFile("titlescreen.png")) {
        std::cout << "ERROR: Could not load titlescreen.png!" << std::endl;
    }
    titleSprite = sf::Sprite(titleTexture);

    sf::Vector2u imageSize = titleTexture.getSize();
    float scaleX = 800.0f / imageSize.x;
    float scaleY = 600.0f / imageSize.y;
    titleSprite.setScale(sf::Vector2f(scaleX, scaleY));

    btnNormal.setSize(sf::Vector2f(280.0f, 60.0f)); btnNormal.setOrigin(sf::Vector2f(140.0f, 30.0f)); btnNormal.setPosition(sf::Vector2f(400.0f, 200.0f)); btnNormal.setFillColor(sf::Color::Transparent);
    btnEndless = btnNormal; btnEndless.setPosition(sf::Vector2f(400.0f, 280.0f));
    btnHelp = btnNormal;    btnHelp.setPosition(sf::Vector2f(400.0f, 340.0f)); 
    btnMusic = btnNormal;   btnMusic.setPosition(sf::Vector2f(400.0f, 390.0f)); 

    helpText.setCharacterSize(20);
    helpText.setFillColor(sf::Color::White);
    helpText.setLineSpacing(1.5f);
    helpText.setString(
        "TOWER DEFENSE - HOW TO PLAY\n\n"
        "  - Press 1: Equip Cannon Tower (100g)\n"
        "  - Press 2: Equip Sniper Tower (150g)\n"
        "  - Press 3: Equip Machine Gun Tower (120g)\n\n"
        "  - Press 'F' during the game to open the Upgrade Shop!\n\n"
        "  - Left-Click on an empty base to build your equipped tower.\n"
        "  - Defeat enemies to earn more gold.\n"
        "  Survive 5 waves to win Normal Mode, or test your skills in Endless!"
    );
    helpText.setPosition(sf::Vector2f(60.0f, 100.0f));

    backText.setCharacterSize(28); backText.setFillColor(sf::Color::Yellow); backText.setStyle(sf::Text::Bold); backText.setOutlineColor(sf::Color::Black); backText.setOutlineThickness(2.0f); backText.setString("< Back to Menu"); backText.setPosition(sf::Vector2f(20.0f, 20.0f));
    btnBack.setSize(sf::Vector2f(250.0f, 40.0f)); btnBack.setPosition(sf::Vector2f(20.0f, 20.0f)); btnBack.setFillColor(sf::Color::Transparent);

    // ==========================================
    // SETTINGS SCREEN LAYOUT
    // ==========================================
    settingsTitleText.setCharacterSize(50);
    settingsTitleText.setFillColor(sf::Color::Yellow);
    settingsTitleText.setString("MUSIC SETTINGS");
    sf::FloatRect sttBounds = settingsTitleText.getLocalBounds();
    settingsTitleText.setOrigin(sf::Vector2f(sttBounds.size.x / 2.0f, sttBounds.size.y / 2.0f));
    settingsTitleText.setPosition(sf::Vector2f(400.0f, 100.0f));

    volumeText.setCharacterSize(40);
    volumeText.setFillColor(sf::Color::White);
    volumeText.setOutlineColor(sf::Color::Black);
    volumeText.setOutlineThickness(2.0f);
    volumeText.setString("Volume: " + std::to_string((int)currentVolume) + "%");
    sf::FloatRect vtBounds = volumeText.getLocalBounds();
    volumeText.setOrigin(sf::Vector2f(vtBounds.size.x / 2.0f, vtBounds.size.y / 2.0f));
    volumeText.setPosition(sf::Vector2f(400.0f, 300.0f));

    // Volume Down [-] Button
    btnVolDown.setSize(sf::Vector2f(60.0f, 60.0f));
    btnVolDown.setOrigin(sf::Vector2f(30.0f, 30.0f));
    btnVolDown.setPosition(sf::Vector2f(200.0f, 300.0f));
    btnVolDown.setFillColor(sf::Color(60, 60, 60));
    btnVolDown.setOutlineColor(sf::Color::White);
    btnVolDown.setOutlineThickness(2.0f);

    textVolDown.setCharacterSize(50);
    textVolDown.setFillColor(sf::Color::White);
    textVolDown.setString("-");
    sf::FloatRect tvdBounds = textVolDown.getLocalBounds();
    textVolDown.setOrigin(sf::Vector2f(tvdBounds.size.x / 2.0f, tvdBounds.size.y / 2.0f));
    textVolDown.setPosition(sf::Vector2f(200.0f, 285.0f));

    // Volume Up [+] Button
    btnVolUp.setSize(sf::Vector2f(60.0f, 60.0f));
    btnVolUp.setOrigin(sf::Vector2f(30.0f, 30.0f));
    btnVolUp.setPosition(sf::Vector2f(600.0f, 300.0f));
    btnVolUp.setFillColor(sf::Color(60, 60, 60));
    btnVolUp.setOutlineColor(sf::Color::White);
    btnVolUp.setOutlineThickness(2.0f);

    textVolUp.setCharacterSize(40);
    textVolUp.setFillColor(sf::Color::White);
    textVolUp.setString("+");
    sf::FloatRect tvuBounds = textVolUp.getLocalBounds();
    textVolUp.setOrigin(sf::Vector2f(tvuBounds.size.x / 2.0f, tvuBounds.size.y / 2.0f));
    textVolUp.setPosition(sf::Vector2f(600.0f, 295.0f));

    // ==========================================
    // RESTART BUTTON SETUP
    // ==========================================
    btnRestart.setSize(sf::Vector2f(240.0f, 60.0f));
    btnRestart.setOrigin(sf::Vector2f(120.0f, 30.0f));
    btnRestart.setPosition(sf::Vector2f(400.0f, 420.0f));
    btnRestart.setFillColor(sf::Color(60, 60, 60, 200));
    btnRestart.setOutlineColor(sf::Color::White);
    btnRestart.setOutlineThickness(2.0f);

    textRestart.setCharacterSize(30);
    textRestart.setFillColor(sf::Color::White);
    textRestart.setString("Play Again");
    sf::FloatRect trBounds = textRestart.getLocalBounds();
    textRestart.setOrigin(sf::Vector2f(trBounds.size.x / 2.0f, trBounds.size.y / 2.0f));
    textRestart.setPosition(sf::Vector2f(400.0f, 410.0f));

    // ==========================================
    // SETUP CUSTOM HUD ELEMENTS
    // ==========================================
    goldText.setCharacterSize(22); goldText.setFillColor(sf::Color::Yellow); goldText.setStyle(sf::Text::Bold); goldText.setOutlineColor(sf::Color::Black); goldText.setOutlineThickness(2.0f); goldText.setPosition(sf::Vector2f(70.0f, 15.0f));

    // --- HUD WAVE TEXT SETUP ---
    hudWaveText.setCharacterSize(24);
    hudWaveText.setFillColor(sf::Color::White);
    hudWaveText.setStyle(sf::Text::Bold);
    hudWaveText.setOutlineColor(sf::Color::Black);
    hudWaveText.setOutlineThickness(2.0f);

    sf::Image coinImage;
    if (coinImage.loadFromFile("coinsheet.png")) {
        sf::Color bgColor = coinImage.getPixel({ 0, 0 });
        coinImage.createMaskFromColor(bgColor);
        if (coinTexture.loadFromImage(coinImage)) {
            coinSprite = sf::Sprite(coinTexture);
            int totalFrames = 4;
            coinFrameWidth = coinTexture.getSize().x / totalFrames;
            int coinFrameHeight = coinTexture.getSize().y;
            coinSprite.setTextureRect(sf::IntRect({ 0, 0 }, { coinFrameWidth, coinFrameHeight }));
            coinSprite.setScale(sf::Vector2f(36.0f / coinFrameHeight, 36.0f / coinFrameHeight));
            coinSprite.setPosition(sf::Vector2f(24.0f, 13.0f)); 
        }
    }

    if (heartTexture.loadFromFile("heart.png")) {
        heartSprite = sf::Sprite(heartTexture);
        sf::FloatRect heartBounds = heartSprite.getLocalBounds();
        heartSprite.setOrigin(sf::Vector2f(heartBounds.size.x / 2.0f, heartBounds.size.y / 2.0f));
        heartSprite.setScale(sf::Vector2f(0.3f, 0.3f));
    }

    waveBannerBg.setSize(sf::Vector2f(800.0f, 100.0f)); waveBannerBg.setOrigin(sf::Vector2f(400.0f, 50.0f)); waveBannerBg.setPosition(sf::Vector2f(400.0f, 300.0f)); waveBannerBg.setFillColor(sf::Color(0, 0, 0, 0));
    waveText.setCharacterSize(50); waveText.setFillColor(sf::Color(255, 255, 255, 0));

    shopPromptText.setCharacterSize(24); shopPromptText.setFillColor(sf::Color::White); shopPromptText.setOutlineColor(sf::Color::Black); shopPromptText.setOutlineThickness(2.0f); shopPromptText.setStyle(sf::Text::Bold); shopPromptText.setString("Shop [F]");
    sf::FloatRect spBounds = shopPromptText.getLocalBounds(); shopPromptText.setOrigin(sf::Vector2f(spBounds.size.x, spBounds.size.y)); shopPromptText.setPosition(sf::Vector2f(780.0f, 580.0f));

    // ==========================================
    // CLEAN ROW-BASED UPGRADE SHOP UI
    // ==========================================
    shopOverlay.setSize(sf::Vector2f(800.0f, 600.0f)); shopOverlay.setFillColor(sf::Color(0, 0, 0, 230));

    shopTitleText.setCharacterSize(50); shopTitleText.setFillColor(sf::Color::Yellow); shopTitleText.setString("WEAPON UPGRADES");
    sf::FloatRect stBounds = shopTitleText.getLocalBounds(); shopTitleText.setOrigin(sf::Vector2f(stBounds.size.x / 2.0f, 0.0f)); shopTitleText.setPosition(sf::Vector2f(400.0f, 30.0f));

    float uiTargetHeight = 100.0f;

    if (shopCannonTex.loadFromFile("cannon.png")) {
        shopCannonSpr = sf::Sprite(shopCannonTex);
        shopCannonSpr.setOrigin(sf::Vector2f(shopCannonTex.getSize().x / 2.0f, shopCannonTex.getSize().y / 2.0f));
        shopCannonSpr.setScale(sf::Vector2f(uiTargetHeight / shopCannonTex.getSize().y, uiTargetHeight / shopCannonTex.getSize().y));
        shopCannonSpr.setPosition(sf::Vector2f(200.0f, 180.0f));
    }
    if (shopSniperTex.loadFromFile("sniper.png")) {
        shopSniperSpr = sf::Sprite(shopSniperTex);
        shopSniperSpr.setOrigin(sf::Vector2f(shopSniperTex.getSize().x / 2.0f, shopSniperTex.getSize().y / 2.0f));
        shopSniperSpr.setScale(sf::Vector2f(uiTargetHeight / shopSniperTex.getSize().y, uiTargetHeight / shopSniperTex.getSize().y));
        shopSniperSpr.setPosition(sf::Vector2f(200.0f, 330.0f));
    }
    if (shopMgTex.loadFromFile("machine_gun.png")) {
        shopMgSpr = sf::Sprite(shopMgTex);
        shopMgSpr.setOrigin(sf::Vector2f(shopMgTex.getSize().x / 2.0f, shopMgTex.getSize().y / 2.0f));
        shopMgSpr.setScale(sf::Vector2f(uiTargetHeight / shopMgTex.getSize().y, uiTargetHeight / shopMgTex.getSize().y));
        shopMgSpr.setPosition(sf::Vector2f(200.0f, 480.0f));
    }

    btnUpCannon.setSize(sf::Vector2f(180.0f, 50.0f)); btnUpCannon.setOrigin(sf::Vector2f(90.0f, 25.0f)); btnUpCannon.setPosition(sf::Vector2f(660.0f, 180.0f)); btnUpCannon.setFillColor(sf::Color(50, 150, 50));
    btnUpSniper.setSize(sf::Vector2f(180.0f, 50.0f)); btnUpSniper.setOrigin(sf::Vector2f(90.0f, 25.0f)); btnUpSniper.setPosition(sf::Vector2f(660.0f, 330.0f)); btnUpSniper.setFillColor(sf::Color(50, 150, 50));
    btnUpMg.setSize(sf::Vector2f(180.0f, 50.0f));     btnUpMg.setOrigin(sf::Vector2f(90.0f, 25.0f));     btnUpMg.setPosition(sf::Vector2f(660.0f, 480.0f));     btnUpMg.setFillColor(sf::Color(50, 150, 50));

    textUpCannon.setCharacterSize(20); textUpCannon.setFillColor(sf::Color::White);
    textUpSniper.setCharacterSize(20); textUpSniper.setFillColor(sf::Color::White);
    textUpMg.setCharacterSize(20);     textUpMg.setFillColor(sf::Color::White);

    // ==========================================
    // SETUP STATIC BASES
    // ==========================================
    if (baseTexture.loadFromFile("base.png")) {
        std::vector<sf::Vector2f> basePositions = {
            sf::Vector2f(192.f, 96.f), sf::Vector2f(207.f, 303.f),
            sf::Vector2f(378.f, 298.f), sf::Vector2f(647.f, 366.f), sf::Vector2f(236.f, 523.f)
        };
        for (const auto& pos : basePositions) {
            sf::Sprite newBase(baseTexture);
            sf::FloatRect bounds = newBase.getLocalBounds();
            newBase.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
            newBase.setPosition(pos);
            newBase.setScale(sf::Vector2f(0.35f, 0.35f));
            bases.push_back(newBase);
        }
    }
}

void Game::run() {
    sf::Clock spawnClock;

    while (window.isOpen()) {
        sf::Time deltaTime = gameClock.restart();
        processEvents();

        if (currentState == GameState::PLAYING) {
            sf::Time spawnDelay = sf::seconds(std::max(0.5f, 2.0f - (waveNumber * 0.3f)));

            if (spawnClock.getElapsedTime() > spawnDelay && enemiesSpawnedThisWave < totalEnemiesThisWave) {
                int enemyType = rand() % 4;

                if (enemyType == 0) enemies.push_back(std::make_unique<PoisonFrog>(gameMap.getPath()));
                else if (enemyType == 1) enemies.push_back(std::make_unique<TribalWarrior>(gameMap.getPath()));
                else if (enemyType == 2) enemies.push_back(std::make_unique<BeetleEnemy>(gameMap.getPath()));
                else if (enemyType == 3) enemies.push_back(std::make_unique<FlyingEnemy>(gameMap.getPath()));

                enemiesSpawnedThisWave++;
                spawnClock.restart();
            }

            if (enemiesSpawnedThisWave >= totalEnemiesThisWave && enemies.empty()) {
                waveNumber++;
                if (!isEndlessMode && waveNumber > 5) {
                    currentState = GameState::GAMEOVER;
                }
                else {
                    enemiesSpawnedThisWave = 0; totalEnemiesThisWave = 10 + (waveNumber * 5); gold += 150;
                    waveText.setString("Wave " + std::to_string(waveNumber));
                    sf::FloatRect wb = waveText.getLocalBounds();
                    waveText.setOrigin(sf::Vector2f(wb.size.x / 2.0f, wb.size.y / 2.0f));
                    waveBannerTimer = 3.0f;
                }
            }

            if (lives <= 0) currentState = GameState::GAMEOVER;
            update(deltaTime);
        }
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) { window.close(); }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::F) {
                if (currentState == GameState::PLAYING) currentState = GameState::SHOP;
                else if (currentState == GameState::SHOP) currentState = GameState::PLAYING;
            }

            if (currentState == GameState::PLAYING) {
                if (keyPressed->code == sf::Keyboard::Key::Num1) { selectedTowerType = 1; equippedText.setString("Equipped: Cannon Tower"); equippedText.setFillColor(sf::Color::White); equipMessageTimer = 2.0f; }
                else if (keyPressed->code == sf::Keyboard::Key::Num2) { selectedTowerType = 2; equippedText.setString("Equipped: Sniper Tower"); equippedText.setFillColor(sf::Color::White); equipMessageTimer = 2.0f; }
                else if (keyPressed->code == sf::Keyboard::Key::Num3) { selectedTowerType = 3; equippedText.setString("Equipped: Machine Gun Tower"); equippedText.setFillColor(sf::Color::White); equipMessageTimer = 2.0f; }
                sf::FloatRect textBounds = equippedText.getLocalBounds(); equippedText.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f)); equippedText.setPosition(sf::Vector2f(400.0f, 550.0f));
            }
        }

        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButton->button == sf::Mouse::Button::Left) {
                float rawX = (float)mouseButton->position.x;
                float rawY = (float)mouseButton->position.y;
                sf::Vector2f mousePos(rawX, rawY);

                if (currentState == GameState::MENU) {
                    if (btnNormal.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        isEndlessMode = false; currentState = GameState::PLAYING; waveText.setString("Wave 1");
                        sf::FloatRect wb = waveText.getLocalBounds(); waveText.setOrigin(sf::Vector2f(wb.size.x / 2.0f, wb.size.y / 2.0f)); waveBannerTimer = 3.0f;
                    }
                    else if (btnEndless.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        isEndlessMode = true; currentState = GameState::PLAYING; waveText.setString("Wave 1");
                        sf::FloatRect wb = waveText.getLocalBounds(); waveText.setOrigin(sf::Vector2f(wb.size.x / 2.0f, wb.size.y / 2.0f)); waveBannerTimer = 3.0f;
                    }
                    else if (btnHelp.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        currentState = GameState::HELP;
                    }
                    else if (btnMusic.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        currentState = GameState::SETTINGS;
                    }
                }
                else if (currentState == GameState::HELP) {
                    if (btnBack.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        currentState = GameState::MENU;
                    }
                }
                else if (currentState == GameState::SETTINGS) {
                    if (btnBack.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        currentState = GameState::MENU;
                    }
                    else if (btnVolDown.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        currentVolume -= 20.0f;
                        if (currentVolume < 0.0f) currentVolume = 0.0f;

                        bgMusic.setVolume(currentVolume);
                        if (currentVolume == 0.0f) volumeText.setString("Volume: MUTED");
                        else volumeText.setString("Volume: " + std::to_string((int)currentVolume) + "%");

                        sf::FloatRect vtBounds = volumeText.getLocalBounds();
                        volumeText.setOrigin(sf::Vector2f(vtBounds.size.x / 2.0f, vtBounds.size.y / 2.0f));
                        volumeText.setPosition(sf::Vector2f(400.0f, 300.0f));
                    }
                    else if (btnVolUp.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        currentVolume += 20.0f;
                        if (currentVolume > 100.0f) currentVolume = 100.0f;

                        bgMusic.setVolume(currentVolume);
                        volumeText.setString("Volume: " + std::to_string((int)currentVolume) + "%");

                        sf::FloatRect vtBounds = volumeText.getLocalBounds();
                        volumeText.setOrigin(sf::Vector2f(vtBounds.size.x / 2.0f, vtBounds.size.y / 2.0f));
                        volumeText.setPosition(sf::Vector2f(400.0f, 300.0f));
                    }
                }
                else if (currentState == GameState::GAMEOVER) {
                    // --- RESTART BUTTON LOGIC ---
                    if (btnRestart.getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        resetGame();
                    }
                }
                else if (currentState == GameState::SHOP) {
                    if (btnUpCannon.getGlobalBounds().contains(mousePos) && gold >= cannonUpCost && cannonLevel < 3) {
                        gold -= cannonUpCost; cannonLevel++; cannonDamage += 15.0f; cannonUpCost += 50;
                        for (auto& tower : towers) if (dynamic_cast<CannonTower*>(tower.get())) tower->setDamage(cannonDamage);
                    }
                    else if (btnUpSniper.getGlobalBounds().contains(mousePos) && gold >= sniperUpCost && sniperLevel < 3) {
                        gold -= sniperUpCost; sniperLevel++; sniperDamage += 75.0f; sniperUpCost += 75;
                        for (auto& tower : towers) if (dynamic_cast<SniperTower*>(tower.get())) tower->setDamage(sniperDamage);
                    }
                    else if (btnUpMg.getGlobalBounds().contains(mousePos) && gold >= mgUpCost && mgLevel < 3) {
                        gold -= mgUpCost; mgLevel++; mgDamage += 5.0f; mgUpCost += 40;
                        for (auto& tower : towers) if (dynamic_cast<MachineGunTower*>(tower.get())) tower->setDamage(mgDamage);
                    }
                }
                else if (currentState == GameState::PLAYING) {
                    bool clickedOnBase = false; sf::Vector2f targetBasePos;
                    for (const auto& base : bases) {
                        if (base.getGlobalBounds().contains(mousePos)) { clickedOnBase = true; targetBasePos = base.getPosition(); break; }
                    }

                    if (clickedOnBase) {
                        bool isOccupied = false;

                        float offsetX = -8.0f;
                        float offsetY = -14.0f;
                        sf::Vector2f perfectPos(targetBasePos.x + offsetX, targetBasePos.y + offsetY);

                        for (const auto& tower : towers) {
                            if (tower->getPosition() == perfectPos) { isOccupied = true; break; }
                        }

                        if (!isOccupied) {
                            int cost = 0;
                            if (selectedTowerType == 1) cost = 100; else if (selectedTowerType == 2) cost = 150; else if (selectedTowerType == 3) cost = 120;

                            if (gold >= cost) {
                                if (selectedTowerType == 1) { auto newTower = std::make_unique<CannonTower>(perfectPos); newTower->setDamage(cannonDamage); newTower->setShootBuffer(cannonBuf); towers.push_back(std::move(newTower)); }
                                else if (selectedTowerType == 2) { auto newTower = std::make_unique<SniperTower>(perfectPos); newTower->setDamage(sniperDamage); newTower->setShootBuffer(sniperBuf); towers.push_back(std::move(newTower)); }
                                else if (selectedTowerType == 3) { auto newTower = std::make_unique<MachineGunTower>(perfectPos); newTower->setDamage(mgDamage); newTower->setShootBuffer(mgBuf); towers.push_back(std::move(newTower)); }
                                gold -= cost;
                            }
                            else {
                                equippedText.setString("Not enough gold!"); equippedText.setFillColor(sf::Color(255, 50, 50)); equipMessageTimer = 2.0f;
                                sf::FloatRect bounds = equippedText.getLocalBounds(); equippedText.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f)); equippedText.setPosition(sf::Vector2f(400.0f, 550.0f));
                            }
                        }
                        else {
                            equippedText.setString("Base is occupied!"); equippedText.setFillColor(sf::Color(255, 50, 50)); equipMessageTimer = 2.0f;
                            sf::FloatRect bounds = equippedText.getLocalBounds(); equippedText.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f)); equippedText.setPosition(sf::Vector2f(400.0f, 550.0f));
                        }
                    }
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    coinAnimTimer += deltaTime.asSeconds();
    if (coinAnimTimer >= 0.15f) {
        currentCoinFrame++;
        if (currentCoinFrame >= 4) currentCoinFrame = 0;
        coinSprite.setTextureRect(sf::IntRect({ currentCoinFrame * coinFrameWidth, 0 }, { coinFrameWidth, static_cast<int>(coinTexture.getSize().y) }));
        coinAnimTimer = 0.0f;
    }

    if (equipMessageTimer > 0.0f) {
        equipMessageTimer -= deltaTime.asSeconds();
        std::uint8_t alpha = 0;
        if (equipMessageTimer > 1.8f) alpha = static_cast<std::uint8_t>(((2.0f - equipMessageTimer) / 0.2f) * 255.0f);
        else if (equipMessageTimer > 0.8f) alpha = 255;
        else if (equipMessageTimer > 0.0f) alpha = static_cast<std::uint8_t>((equipMessageTimer / 0.8f) * 255.0f);
        else equipMessageTimer = 0.0f;
        sf::Color currentColor = equippedText.getFillColor(); equippedText.setFillColor(sf::Color(currentColor.r, currentColor.g, currentColor.b, alpha)); equippedText.setOutlineColor(sf::Color(0, 0, 0, alpha));
    }

    if (waveBannerTimer > 0.0f) {
        waveBannerTimer -= deltaTime.asSeconds(); float t = 3.0f - waveBannerTimer; float currentX = 0; std::uint8_t alpha = 0; std::uint8_t bgAlpha = 0;
        if (t < 0.5f) { float p = t / 0.5f; currentX = -100.0f + (500.0f * p); alpha = static_cast<std::uint8_t>(p * 255.0f); bgAlpha = static_cast<std::uint8_t>(p * 150.0f); }
        else if (t < 2.5f) { float p = (t - 0.5f) / 2.0f; currentX = 400.0f + (50.0f * p); alpha = 255; bgAlpha = 150; }
        else if (t <= 3.0f) { float p = (t - 2.5f) / 0.5f; currentX = 450.0f + (450.0f * p); alpha = static_cast<std::uint8_t>((1.0f - p) * 255.0f); bgAlpha = static_cast<std::uint8_t>((1.0f - p) * 150.0f); }
        else waveBannerTimer = 0.0f;
        waveText.setPosition(sf::Vector2f(currentX, 290.0f)); waveText.setFillColor(sf::Color(255, 255, 255, alpha)); waveBannerBg.setFillColor(sf::Color(0, 0, 0, bgAlpha));
    }

    for (auto it = enemies.begin(); it != enemies.end();) {
        (*it)->update(deltaTime);
        if ((*it)->getPosition().x > 780) { lives--; it = enemies.erase(it); }
        else if ((*it)->isDead()) { gold += 20; it = enemies.erase(it); }
        else { ++it; }
    }

    for (auto& tower : towers) { tower->attack(enemies, deltaTime); tower->update(deltaTime); tower->updateProjectiles(deltaTime, enemies); }
}

void Game::render() {
    
    window.clear(sf::Color(45, 85, 30));

    if (currentState == GameState::MENU) {
        window.draw(titleSprite);
    }
    else if (currentState == GameState::HELP) {
        window.draw(titleSprite); sf::RectangleShape darkOverlay(sf::Vector2f(800.0f, 600.0f)); darkOverlay.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(darkOverlay); window.draw(helpText); window.draw(backText);
    }
    else if (currentState == GameState::SETTINGS) {
        window.draw(titleSprite);
        sf::RectangleShape darkOverlay(sf::Vector2f(800.0f, 600.0f));
        darkOverlay.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(darkOverlay);

        window.draw(settingsTitleText);
        window.draw(backText);
        window.draw(btnVolDown);
        window.draw(textVolDown);
        window.draw(btnVolUp);
        window.draw(textVolUp);
        window.draw(volumeText);
    }
    
    else if (currentState == GameState::GAMEOVER) {
        gameMap.render(window);
        for (const auto& base : bases) window.draw(base);
        for (const auto& tower : towers) tower->render(window);

        sf::RectangleShape darkOverlay(sf::Vector2f(800.0f, 600.0f));
        darkOverlay.setFillColor(sf::Color(0, 0, 0, 210));
        window.draw(darkOverlay);

        if (lives > 0) {
            titleText.setString("VICTORY");
            titleText.setFillColor(sf::Color(255, 215, 0));
        }
        else {
            titleText.setString("DEFEAT");
            titleText.setFillColor(sf::Color(220, 20, 20));
        }
        titleText.setCharacterSize(80);
        sf::FloatRect ttBounds = titleText.getLocalBounds();
        titleText.setOrigin(sf::Vector2f(ttBounds.size.x / 2.0f, ttBounds.size.y / 2.0f));
        titleText.setPosition(sf::Vector2f(400.0f, 180.0f));
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setOutlineThickness(4.0f);
        window.draw(titleText);

        gameOverSubText.setString("You reached Wave " + std::to_string(waveNumber));
        gameOverSubText.setCharacterSize(30);
        gameOverSubText.setFillColor(sf::Color::White);
        sf::FloatRect stBounds = gameOverSubText.getLocalBounds();
        gameOverSubText.setOrigin(sf::Vector2f(stBounds.size.x / 2.0f, stBounds.size.y / 2.0f));
        gameOverSubText.setPosition(sf::Vector2f(400.0f, 280.0f));
        window.draw(gameOverSubText);

        window.draw(btnRestart);
        window.draw(textRestart);
    }
    else if (currentState == GameState::PLAYING || currentState == GameState::SHOP) {
        gameMap.render(window);
        for (const auto& base : bases) window.draw(base);
        for (const auto& tower : towers) tower->render(window);
        for (const auto& enemy : enemies) enemy->render(window);

        goldText.setString("Gold: " + std::to_string(gold));
        window.draw(coinSprite); window.draw(goldText);

        
        for (int i = 0; i < lives; ++i) { heartSprite.setPosition(sf::Vector2f(770.0f - (i * 40.0f), 25.0f)); window.draw(heartSprite); }

        if (isEndlessMode) {
            hudWaveText.setString("Wave: " + std::to_string(waveNumber));
        }
        else {
            hudWaveText.setString("Wave: " + std::to_string(waveNumber) + " / 5");
        }
        sf::FloatRect hwBounds = hudWaveText.getLocalBounds();
        hudWaveText.setOrigin(sf::Vector2f(hwBounds.size.x / 2.0f, 0.0f));
        hudWaveText.setPosition(sf::Vector2f(400.0f, 15.0f));
        window.draw(hudWaveText);

        if (currentState == GameState::PLAYING) window.draw(shopPromptText);
        if (equipMessageTimer > 0.0f) window.draw(equippedText);
        if (waveBannerTimer > 0.0f) { window.draw(waveBannerBg); window.draw(waveText); }

        if (currentState == GameState::SHOP) {
            window.draw(shopOverlay); window.draw(shopTitleText); window.draw(shopCannonSpr); window.draw(shopSniperSpr); window.draw(shopMgSpr);

            if (cannonLevel < 3) { textUpCannon.setString("Upgrade " + std::to_string(cannonLevel + 1) + "\nCost: " + std::to_string(cannonUpCost)); btnUpCannon.setFillColor(sf::Color(50, 150, 50)); }
            else { textUpCannon.setString("MAXED"); btnUpCannon.setFillColor(sf::Color(100, 100, 100)); }

            if (sniperLevel < 3) { textUpSniper.setString("Upgrade " + std::to_string(sniperLevel + 1) + "\nCost: " + std::to_string(sniperUpCost)); btnUpSniper.setFillColor(sf::Color(50, 150, 50)); }
            else { textUpSniper.setString("MAXED"); btnUpSniper.setFillColor(sf::Color(100, 100, 100)); }

            if (mgLevel < 3) { textUpMg.setString("Upgrade " + std::to_string(mgLevel + 1) + "\nCost: " + std::to_string(mgUpCost)); btnUpMg.setFillColor(sf::Color(50, 150, 50)); }
            else { textUpMg.setString("MAXED"); btnUpMg.setFillColor(sf::Color(100, 100, 100)); }

            window.draw(btnUpCannon); window.draw(btnUpSniper); window.draw(btnUpMg);

            sf::FloatRect cBounds = textUpCannon.getLocalBounds(); textUpCannon.setOrigin(sf::Vector2f(cBounds.size.x / 2.0f, cBounds.size.y / 2.0f)); textUpCannon.setPosition(sf::Vector2f(660.0f, 175.0f));
            sf::FloatRect sBounds = textUpSniper.getLocalBounds(); textUpSniper.setOrigin(sf::Vector2f(sBounds.size.x / 2.0f, sBounds.size.y / 2.0f)); textUpSniper.setPosition(sf::Vector2f(660.0f, 325.0f));
            sf::FloatRect mBounds = textUpMg.getLocalBounds();     textUpMg.setOrigin(sf::Vector2f(mBounds.size.x / 2.0f, mBounds.size.y / 2.0f));     textUpMg.setPosition(sf::Vector2f(660.0f, 475.0f));
            window.draw(textUpCannon); window.draw(textUpSniper); window.draw(textUpMg);

            sf::Text statLabelC(uiFont); statLabelC.setCharacterSize(22); statLabelC.setFillColor(sf::Color::White); statLabelC.setString("Current Dmg: " + std::to_string((int)cannonDamage)); statLabelC.setPosition(sf::Vector2f(280.0f, 165.0f));
            sf::Text statLabelS(uiFont); statLabelS.setCharacterSize(22); statLabelS.setFillColor(sf::Color::White); statLabelS.setString("Current Dmg: " + std::to_string((int)sniperDamage)); statLabelS.setPosition(sf::Vector2f(280.0f, 315.0f));
            sf::Text statLabelM(uiFont); statLabelM.setCharacterSize(22); statLabelM.setFillColor(sf::Color::White); statLabelM.setString("Current Dmg: " + std::to_string((int)mgDamage)); statLabelM.setPosition(sf::Vector2f(280.0f, 465.0f));
            window.draw(statLabelC); window.draw(statLabelS); window.draw(statLabelM);
        }
    }
    window.display();
}


void Game::resetGame() {
    towers.clear();
    enemies.clear();

    gold = 500;
    lives = 3;
    waveNumber = 1;
    enemiesSpawnedThisWave = 0;
    totalEnemiesThisWave = 10;

    cannonDamage = 35.0f;  cannonUpCost = 150; cannonLevel = 0;
    sniperDamage = 150.0f; sniperUpCost = 150; sniperLevel = 0;
    mgDamage = 10.0f;      mgUpCost = 120;     mgLevel = 0;

    currentState = GameState::MENU;
}