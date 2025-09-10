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
    Texture2D playerTexture[] = {
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_03_png_processed.png"),
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_01_png_processed.png"),
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_02Right_png_processed 1.png"),
    };

    InitAudioDevice();

    SetTargetFPS(60);

    //Game start sound
    Sound game_start_sound = LoadSound("assets/sounds/level-start-sfx.wav");
    PlaySound(game_start_sound);

    auto ship = Ship(playerTexture, Vector2(screenWidth / 2 - 40, screenHeight * 0.85),WHITE, 5);
    Sound player_bullet = LoadSound("assets/sounds/player-bullet-sfx.wav");

    std::vector<Bullet> playerBullets;


    // ALiens initialization.
    std::vector<Alien> aliens;

    Texture2D alienTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/alien.png");

    Sound alienBulletSound = LoadSound("assets/sounds/alien-bullet-sfx.wav");

    std::vector<Bullet> alienBullets;
    float alien_bullet_spawn_timer = 20;

    // Create a random engine
    std::random_device rd;
    std::mt19937 gen(rd());

    int rows = 4;
    int cols = 8;
    aliens.reserve(rows * cols);

    for (int i = 0; i < rows; ++i)
    {
        // if (static_cast<float>(i) / rows < 0.5f)
        // {
        //     alienTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/alien1.png");
        //     std::cout << i << std::endl;
        // }
        // else
        // {
        //     alienTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/alien0.png");
        // }
        for (int j = 0; j < cols; ++j)
        {
            aliens.emplace_back(alienTexture, Vector2(screenWidth / rows, screenHeight / cols), i, j);
        }
    }


    while (!WindowShouldClose())
    {
        // Input
        if (IsKeyPressed(KEY_SPACE))
        {
            Vector2 bulletStart = {ship.GetPos().x + 25, ship.GetPos().y - 5};
            playerBullets.emplace_back(bulletStart, 10.0f, playerBulletTexture, -1);
            PlaySound(player_bullet);
        }

        // Update
        ship.Update();

        // Update bullets
        for (auto& playerBullet : playerBullets)
        {
            playerBullet.Update();
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
            alien.Update(cols);
        }

        //Spawn alien bullet after delay.
        alien_bullet_spawn_timer -= 0.2f;
        if (alien_bullet_spawn_timer <= 0 && !aliens.empty())
        {
            std::uniform_int_distribution<> dist(0, static_cast<int>(aliens.size()) - 1);
            int random_alien = dist(gen);
            for (int i = 0; i < random_alien / 5; ++i)
            {
                Vector2 alienBullet = {
                    aliens[i].GetAlienPosX() + 10,
                    aliens[i].GetAlienPosY() - 40
                };

                //Random speed
                std::uniform_int_distribution<> speed(3, 5);
                int alien_speed = speed(gen);

                alienBullets.emplace_back(alienBullet, alien_speed, alienBulletTexture, +1);
            }
            PlaySound(alienBulletSound);

            alien_bullet_spawn_timer = 20; // reset timer
        }


        // Update alien's bullet
        for (auto& alien_bullet : alienBullets)
        {
            alien_bullet.Update();

            Rectangle alienRec = {
                alien_bullet.GetPosition().x, alien_bullet.GetPosition().y, alien_bullet.GetWidth(),
                alien_bullet.GetHeight()
            };
            ship.CheckCollision(alienRec);
        }

        alienBullets.erase(
            std::remove_if(alienBullets.begin(), alienBullets.end(),
                           [](const Bullet& b) { return !b.IsActive(); }),
            alienBullets.end()
        );

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
            alien.Draw();
        }
        // Draw Alien's Bullets
        for (auto& alienBullet : alienBullets)
        {
            alienBullet.Draw();
        }

        EndDrawing();
    }

    // De-Initialization

    //Unload textures
    UnloadTexture(background_texture);

    UnloadTexture(playerBulletTexture);

    for (int i = 0; i < NUM_SHIP_FRAME; ++i)
    {
        UnloadTexture(playerTexture[i]);
    }

    UnloadTexture(alienTexture);

    // Unload Sounds
    UnloadSound(game_start_sound);

    UnloadSound(player_bullet);

    for (auto& alien : aliens)
    {
        alien.UnloadMovementSound();
    }

    UnloadSound(alienBulletSound);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
