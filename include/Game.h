#pragma once
#include <vector>
#include <random>
#include <memory>
#include <raylib.h>

#include "../include/Ship.h"
#include "../include/Alien.h"
#include "../include/Bullet.h"

class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();

    void Run(); // runs the main loop

private:
    // lifecycle
    void Init();
    void Update();
    void Draw();
    void Cleanup();

    // subsystems
    void UpdateInput();
    void UpdatePlayerBullets();
    void UpdateAlienBullets();
    void UpdateAliens();
    void SpawnAlienBullets();
    bool AllAliensDead() const;

    void RemoveInactiveBullets();
    void RemoveInactiveAliens();

    void SpawnInitialAliens(int rows, int cols);
    void ResetLevel();

private:
    const int screenWidth;
    const int screenHeight;

    // Assets
    Texture2D backgroundTexture{};
    Rectangle srcRec{};
    Rectangle dstRec{};

    // Player
    Texture2D shipTexture[NUM_SHIP_FRAME]{};
    std::unique_ptr<Ship> ship;

    // Bullets
    Texture2D playerBulletTexture{};
    Texture2D alienBulletTexture{};

    // Aliens
    Texture2D alienTexture{};
    std::vector<Alien> aliens;

    // Sounds
    Sound gameStartSound{};
    Sound shipExplosionSound{};
    Sound shipBulletSound{};
    Sound alienExplosionSound{};
    Sound alienBulletSound{};
    Sound gameOverSound{};

    // runtime lists
    std::vector<Bullet> playerBullets;
    std::vector<Bullet> alienBullets;

    // timers / state
    float alien_bullet_spawn_timer = 20.0f;
    bool isPlayerWin = false;
    bool hasGameOverSoundPlayed = false;

    // UI
    Rectangle button{};
    bool buttonPressed = false;

    // level config
    int rows = 2;
    int cols = 6;

    // randomness
    std::random_device rd;
    std::mt19937 gen;

};
