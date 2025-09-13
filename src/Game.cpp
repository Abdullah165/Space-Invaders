#include "../include/Game.h"

#include <chrono>

Game::Game(int screenW, int screenH)
    : screenWidth(screenW)
      , screenHeight(screenH)
      , gen(rd())
{
    Init();
}

Game::~Game()
{
    Cleanup();
}

void Game::Init()
{
    InitWindow(screenWidth, screenHeight, "Space Invaders");
    InitAudioDevice();
    SetTargetFPS(60);

    // Background
    backgroundTexture = LoadTexture("assets/textures/PixelSpaceRage/PixelBackgroundSeamlessVertically.png");
    srcRec = {0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height};
    dstRec = {0.0f, 0.0f, (float)screenWidth, (float)screenHeight};

    // Player bullet texture
    playerBulletTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/Minigun_Medium_png_processed.png");
    // Alien bullet texture
    alienBulletTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/bullet.png");

    // Ship textures (keep the exact paths you had)
    shipTexture[0] = LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_03_png_processed.png");
    shipTexture[1] = LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_01_png_processed.png");
    shipTexture[2] = LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_02Right_png_processed 1.png");

    // Sounds
    gameStartSound = LoadSound("assets/sounds/level-start-sfx.wav");
    shipExplosionSound = LoadSound("assets/sounds/player-explosion-sfx.wav");
    shipBulletSound = LoadSound("assets/sounds/player-bullet-sfx.wav");
    alienExplosionSound = LoadSound("assets/sounds/alien-explosion-sfx.wav");
    alienBulletSound = LoadSound("assets/sounds/alien-bullet-sfx.wav");
    gameOverSound = LoadSound("assets/sounds/game-over-sfx.wav");

    PlaySound(gameStartSound);

    // Create ship AFTER loading textures (use unique_ptr to avoid initialization issues)
    ship = std::make_unique<Ship>(shipTexture, Vector2{screenWidth / 2 - 40.0f, screenHeight * 0.85f}, WHITE, 5);

    // Alien texture
    alienTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/alien.png");

    // UI button
    button = {(float)screenWidth / 2 - 100.0f, (float)screenHeight / 2, 150.0f, 50.0f};

    // Spawn initial aliens
    SpawnInitialAliens(rows, cols);
}

void Game::SpawnInitialAliens(int rows_, int cols_)
{
    rows = rows_;
    cols = cols_;
    aliens.clear();
    aliens.reserve(rows * cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            aliens.emplace_back(alienTexture, Vector2(screenWidth / rows, screenHeight / cols), i, j);
        }
    }
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        Update();

        BeginDrawing();
        Draw();
        EndDrawing();
    }
}

void Game::Update()
{
    // Input-related actions
    UpdateInput();

    // Update actors
    ship->Update();
    UpdatePlayerBullets();
    UpdateAlienBullets();
    UpdateAliens();

    // Spawn enemy bullets if any alien alive
    SpawnAlienBullets();

    // Remove inactive objects
    RemoveInactiveBullets();
    RemoveInactiveAliens();
}

void Game::UpdateInput()
{
    if (IsKeyPressed(KEY_SPACE) && !ship->HasDead())
    {
        Vector2 bulletStart = {ship->GetPos().x + 25.0f, ship->GetPos().y - 5.0f};
        playerBullets.emplace_back(bulletStart, 10.0f, playerBulletTexture, -1); // -1 moves upward
        PlaySound(shipBulletSound);
    }

    // play again
    if ((CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || isPlayerWin)
    {
        buttonPressed = true;
        isPlayerWin = false;
        ship->SetNumOfLives(3);
        aliens.clear();
        alienBullets.clear();
        playerBullets.clear();
        hasGameOverSoundPlayed = false;

        // Increase level
        rows += 1;
        cols += 1;
        if (rows > 6) rows = 6;
        if (cols > 8) cols = 8;

        SpawnInitialAliens(rows, cols);
    }
}

void Game::UpdatePlayerBullets()
{
    for (auto& b : playerBullets)
    {
        b.Update();

        Rectangle shipBulletRec = {
            b.GetPosition().x, b.GetPosition().y,
            b.GetWidth(), b.GetHeight()
        };

        // check collision with aliens
        for (auto& a : aliens)
        {
            if (a.CheckCollision(shipBulletRec))
            {
                b.SetActive(false);
                PlaySound(alienExplosionSound);
            }
        }
    }
}

void Game::UpdateAlienBullets()
{
    for (auto& b : alienBullets)
    {
        b.Update();

        Rectangle alienBulletRec = {
            b.GetPosition().x, b.GetPosition().y,
            b.GetWidth(), b.GetHeight()
        };

        if (ship->CheckCollision(alienBulletRec))
        {
            b.SetActive(false);
        }
    }
}

void Game::UpdateAliens()
{
    for (auto& a : aliens)
    {
        if (ship->HasDead()) break;
        a.Update(cols);
    }
}

bool Game::AllAliensDead() const
{
    for (const auto& a : aliens)
    {
        if (a.IsActive()) return false;
    }
    return true;
}

void Game::SpawnAlienBullets()
{
    // Decrease timer
    alien_bullet_spawn_timer -= 0.15f;

    if (alien_bullet_spawn_timer > 0.0f) return;
    if (aliens.empty()) return;
    if (AllAliensDead()) return;

    // gather alive indices so we only pick an alive alien
    std::vector<int> aliveIndices;
    aliveIndices.reserve(aliens.size());
    for (size_t i = 0; i < aliens.size(); ++i)
    {
        if (aliens[i].IsActive()) aliveIndices.push_back((int)i);
    }

    if (aliveIndices.empty())
    {
        alien_bullet_spawn_timer = 20.0f;
        return;
    }

    std::uniform_int_distribution<> dist(0, (int)aliveIndices.size() - 1);
    int chosen = aliveIndices[dist(gen)];
    Vector2 pos = {aliens[chosen].GetAlienPosX() + 10.0f, aliens[chosen].GetAlienPosY() - 40.0f};

    std::uniform_int_distribution<> speedDist(5, 6);
    int alien_speed = speedDist(gen);

    alienBullets.emplace_back(pos, (float)alien_speed, alienBulletTexture, +1); // downwards

    PlaySound(alienBulletSound);
    alien_bullet_spawn_timer = 20.0f;
}

void Game::RemoveInactiveBullets()
{
    // remove player bullets
    for (int i = (int)playerBullets.size() - 1; i >= 0; --i)
    {
        if (!playerBullets[i].IsActive()) playerBullets.erase(playerBullets.begin() + i);
    }

    // remove alien bullets
    for (int i = (int)alienBullets.size() - 1; i >= 0; --i)
    {
        if (!alienBullets[i].IsActive()) alienBullets.erase(alienBullets.begin() + i);
    }
}

void Game::RemoveInactiveAliens()
{
    if (AllAliensDead())
    {
        aliens.clear();
    }

    // if all dead -> player win
    if (aliens.empty())
    {
        isPlayerWin = true;
    }
}

void Game::Draw()
{
    ClearBackground(BLACK);
    DrawTexturePro(backgroundTexture, srcRec, dstRec, Vector2{0.0f, 0.0f}, 0.0f, WHITE);

    ship->Draw();

    for (auto& b : playerBullets) b.Draw();

    for (auto& a : aliens)
    {
        if (ship->HasDead()) break;
        a.Draw();
    }

    for (auto& b : alienBullets)
    {
        if (ship->HasDead()) break;
        b.Draw();
    }

    DrawText(TextFormat("Lives: %i", ship->GetNumOfLives()), 50, 15, 30, WHITE);

    if (ship->HasDead())
    {
        DrawRectangleRec(button, buttonPressed ? RED : WHITE);
        DrawText("Play Again", screenWidth / 2 - 80, (int)(screenHeight / 1.9f), 20, BLACK);
        DrawText("Game Over", 370, screenHeight / 2 - 200, 100, WHITE);

        if (!hasGameOverSoundPlayed)
        {
            PlaySound(shipExplosionSound);
            PlaySound(gameOverSound);
            hasGameOverSoundPlayed = true;
        }
    }
}

void Game::Cleanup()
{
    // unload textures & sounds
    UnloadTexture(backgroundTexture);
    UnloadTexture(playerBulletTexture);
    UnloadTexture(alienBulletTexture);

    for (int i = 0; i < NUM_SHIP_FRAME; ++i)
    {
        UnloadTexture(shipTexture[i]);
    }

    UnloadTexture(alienTexture);

    UnloadSound(gameStartSound);
    UnloadSound(shipBulletSound);
    UnloadSound(shipExplosionSound);
    UnloadSound(alienExplosionSound);
    UnloadSound(alienBulletSound);
    UnloadSound(gameOverSound);

    for (auto& a : aliens)
    {
        a.UnloadMovementSound();
    }

    CloseAudioDevice();
    CloseWindow();
}
