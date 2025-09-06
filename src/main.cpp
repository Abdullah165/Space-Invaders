#include <algorithm>
#include <iostream>
#include <raylib.h>
#include <vector>

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
    Texture2D bulletTexture = LoadTexture("assets/textures/PixelSpaceRage/256px/Minigun_Medium_png_processed.png");

    //Player's ship texture
    Texture2D playerTexture[] = {
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_03_png_processed.png"),
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_01_png_processed.png"),
        LoadTexture("assets/textures/PixelSpaceRage/256px/PlayerRed_Frame_02Right_png_processed 1.png"),
    };

    InitAudioDevice();

    SetTargetFPS(60);


    auto ship = Ship(playerTexture, Vector2(screenWidth / 2 - 40, screenHeight * 0.85), 80, 80,WHITE, 10);
    Sound player_bullet = LoadSound("assets/sounds/player-bullet-sfx.wav");

    std::vector<Bullet> bullets;

    // ALiens initialization.
    std::vector<Alien> aliens;

    int rows = 4;
    int cols = 6;
    aliens.reserve(rows * cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            aliens.emplace_back(Vector2(screenWidth / rows, screenHeight / cols), i, j);
        }
    }

    while (!WindowShouldClose())
    {
        // Input
        if (IsKeyPressed(KEY_SPACE))
        {
            Vector2 bulletStart = {ship.GetPos().x + 25, ship.GetPos().y - 5};
            bullets.emplace_back(bulletStart, 10.0f, bulletTexture);
            PlaySound(player_bullet);
        }

        // Update
        ship.Update();

        // Update bullets
        for (auto& bullet : bullets)
        {
            bullet.Update();
        }

        // Update aliens
        for (auto& alien : aliens)
        {
            alien.Update(cols);
        }

        // Remove inactive bullets (off screen) from bullets vector
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [](const Bullet& b) { return !b.IsActive(); }),
            bullets.end()
        );

        //Draw
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(background_texture, sourceRec, destRec, Vector2{0, 0}, 0, WHITE);

        // Draw Ship
        ship.Draw();

        // Draw Bullets
        for (auto& bullet : bullets)
        {
            bullet.Draw();
        }


        // Aliens
        for (auto& alien : aliens)
        {
            alien.Draw();
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(background_texture);

    UnloadTexture(bulletTexture);

    for (int i = 0; i < NUM_SHIP_FRAME; ++i)
    {
        UnloadTexture(playerTexture[i]);
    }

    UnloadSound(player_bullet);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
