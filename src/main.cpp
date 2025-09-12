#include <algorithm>
#include <iostream>
#include <ostream>
#include <raylib.h>
#include <vector>
#include <random>

#include "../include/Bullet.h"
#include "../include/Ship.h"
#include "../include/Alien.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;


    InitWindow(screenWidth, screenHeight, "Space Invaders");

    Texture2D background_texture = LoadTexture("assets/textures/PixelSpaceRage/PixelBackgroundSeamlessVertically.png");
    Rectangle sourceRec = {0, 0, (float)background_texture.width, (float)background_texture.height};
    Rectangle destRec = {0, 0, screenWidth, screenHeight};

    //Player's bullet texture
    Texture2D playerBulletTexture =
        LoadTexture("assets/textures/PixelSpaceRage/256px/Minigun_Medium_png_processed.png");
    Texture2D alienBulletTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/bullet.png");

    //Player's ship texture
    Texture2D shipTexture[] = {
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_03_png_processed.png"),
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_01_png_processed.png"),
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_02Right_png_processed 1.png"),
    };

    InitAudioDevice();

    SetTargetFPS(60);

    //Game start sound
    Sound game_start_sound = LoadSound("assets/sounds/level-start-sfx.wav");
    PlaySound(game_start_sound);

    auto ship = Ship(shipTexture, Vector2(screenWidth / 2 - 40, screenHeight * 0.85),WHITE, 5);
    Sound ship_explosion_sound = LoadSound("assets/sounds/player-explosion-sfx.wav");
    Sound ship_bullet = LoadSound("assets/sounds/player-bullet-sfx.wav");

    std::vector<Bullet> playerBullets;

    // ALiens initialization.
    std::vector<Alien> aliens;

    Texture2D alienTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/alien.png");

    Sound alien_explosion_sound = LoadSound("assets/sounds/alien-explosion-sfx.wav");
    Sound alienBulletSound = LoadSound("assets/sounds/alien-bullet-sfx.wav");

    std::vector<Bullet> alienBullets;
    float alien_bullet_spawn_timer = 20;

    // Create a random engine
    std::random_device rd;
    std::mt19937 gen(rd());

    int rows = 2;
    int cols = 6;
    aliens.reserve(rows * cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            aliens.emplace_back(alienTexture, Vector2(screenWidth / rows, screenHeight / cols), i, j);
        }
    }


    Rectangle button = {screenWidth / 2 - 100, screenHeight / 2, 150, 50};
    bool buttonPressed = false;
    bool isPlayerWin = false;

    Sound game_over_sound = LoadSound("assets/sounds/game-over-sfx.wav");
    bool hasGameOverSoundPlayed = false;

    while (!WindowShouldClose())
    {
        // Input
        if (IsKeyPressed(KEY_SPACE) && !ship.HasDead())
        {
            Vector2 bulletStart = {ship.GetPos().x + 25, ship.GetPos().y - 5};
            playerBullets.emplace_back(bulletStart, 10.0f, playerBulletTexture, -1);
            PlaySound(ship_bullet);
        }

        // Update
        ship.Update();

        // Update bullets
        for (auto& playerBullet : playerBullets)
        {
            playerBullet.Update();

            Rectangle shipBulletRec = {
                playerBullet.GetPosition().x, playerBullet.GetPosition().y, playerBullet.GetWidth(),
                playerBullet.GetHeight()
            };

            for (auto& alien : aliens)
            {
                if (alien.CheckCollision(shipBulletRec))
                {
                    playerBullet.SetActive(false);
                    PlaySound(alien_explosion_sound);
                }
            }
        }

        // Remove inactive bullets (off screen) from bullets vector
        playerBullets.erase(
            std::remove_if(playerBullets.begin(), playerBullets.end(),
                           [](const Bullet& b) { return !b.IsActive(); }),
            playerBullets.end()
        );


        // Update aliens
        for (auto& alien : aliens)
        {
            if (ship.HasDead()) break;
            alien.Update(cols);
        }

        // Check if all aliens dead
        bool allAliensDead = std::all_of(aliens.begin(), aliens.end(),
                                         [](const Alien& alien)
                                         {
                                             return !alien.IsActive();
                                         });

        //Spawn alien bullet after delay.
        alien_bullet_spawn_timer -= 0.15f;
        if (alien_bullet_spawn_timer <= 0 && !allAliensDead)
        {
            std::uniform_int_distribution<> dist(0, static_cast<int>(aliens.size()) - 1);
            int random_alien = dist(gen);
            for (int i = 0; i < random_alien / 4; ++i)
            {
                Vector2 alienBulletPos = {
                    aliens[i].GetAlienPosX() + 10,
                    aliens[i].GetAlienPosY() - 40
                };

                //Random speed
                std::uniform_int_distribution<> speed(5, 6);
                int alienBullet_speed = speed(gen);

                alienBullets.emplace_back(alienBulletPos, alienBullet_speed, alienBulletTexture, +1);
                alienBulletTexture.width * 0.05;
            }
            PlaySound(alienBulletSound);

            alien_bullet_spawn_timer = 20; // reset timer
        }


        // Update alien's bullet
        for (auto& alien_bullet : alienBullets)
        {
            alien_bullet.Update();

            Rectangle alienBulletRec = {
                alien_bullet.GetPosition().x, alien_bullet.GetPosition().y, alien_bullet.GetWidth(),
                alien_bullet.GetHeight()
            };

            if (ship.CheckCollision(alienBulletRec))
            {
                alien_bullet.SetActive(false);
            }
        }

        //Remove the alien_bullet when it's not no longer active.
        alienBullets.erase(
            std::remove_if(alienBullets.begin(), alienBullets.end(),
                           [](const Bullet& b) { return !b.IsActive(); }),
            alienBullets.end()
        );

        if (allAliensDead)
        {
            isPlayerWin = true;
        }

        // Check if the button is pressed
        if ((CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
            isPlayerWin)
        {
            buttonPressed = true;
            isPlayerWin = false;
            ship.SetNumOfLives(3);
            aliens.clear();

            hasGameOverSoundPlayed = false;

            //Increase number of Aliens
            rows += 1;
            cols += 1;

            // Limit the Aliens count
            if (rows > 6 && cols > 8)
            {
                rows = 6;
                cols = 8;
            }

            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < cols; ++j)
                {
                    aliens.emplace_back(alienTexture, Vector2(screenWidth / rows, screenHeight / cols), i, j);
                }
            }
        }

        //Draw
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(background_texture, sourceRec, destRec, Vector2{0, 0}, 0, WHITE);

        // Draw Ship
        ship.Draw();

        // Draw Ship Bullets
        for (auto& bullet : playerBullets)
        {
            bullet.Draw();
        }

        // Aliens
        for (auto& alien : aliens)
        {
            if (ship.HasDead()) break;
            alien.Draw();
        }

        // Draw Alien's Bullets
        for (auto& alienBullet : alienBullets)
        {
            if (ship.HasDead()) break;
            alienBullet.Draw();
        }

        //Draw texts
        DrawText(TextFormat("Lives: %i", ship.GetNumOfLives()), 50, 15, 30, WHITE);


        //Draw buttons
        if (ship.HasDead())
        {
            DrawRectangleRec(button, buttonPressed ? RED : WHITE);

            DrawText("Play Again", screenWidth / 2 - 80, screenHeight / 1.9, 20, BLACK);
            DrawText("Game Over", 370, screenHeight / 2 - 200, 100, WHITE);

            if (!hasGameOverSoundPlayed && ship.HasDead())
            {
                PlaySound(ship_explosion_sound);
                PlaySound(game_over_sound);
                hasGameOverSoundPlayed = true;
            }
        }

        EndDrawing();
    }

    // De-Initialization

    //Unload textures
    UnloadTexture(background_texture);

    UnloadTexture(playerBulletTexture);

    for (int i = 0; i < NUM_SHIP_FRAME; ++i)
    {
        UnloadTexture(shipTexture[i]);
    }

    UnloadTexture(alienTexture);

    // Unload Sounds
    UnloadSound(game_start_sound);

    UnloadSound(ship_bullet);

    for (auto& alien : aliens)
    {
        alien.UnloadMovementSound();
    }

    UnloadSound(alien_explosion_sound);

    UnloadSound(alienBulletSound);

    UnloadSound(ship_explosion_sound);

    UnloadSound(game_over_sound);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
