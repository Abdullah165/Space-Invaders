#include <algorithm>
#include <iostream>
#include <raylib.h>
#include <vector>

#include "../include/Bullet.h"
#include "../include/Ship.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;


    InitWindow(screenWidth, screenHeight, "Space Invaders");

    InitAudioDevice();

    SetTargetFPS(60);


    auto ship = Ship(Vector2(screenWidth / 2 - 40, screenHeight * 0.85), 80, 80,WHITE, 10);

    std::vector<Bullet> bullets;

    Sound level_start = LoadSound("assets/sounds/level-start-sfx.wav");


    while (!WindowShouldClose())
    {
        // Input
        if (IsKeyPressed(KEY_SPACE))
        {
            PlaySound(level_start);
            Vector2 bulletStart = {ship.GetPos().x + 35, ship.GetPos().y - 25};
            bullets.emplace_back(bulletStart, 10.0f);
        }

        // Update
        ship.Update();

        // Update bullets
        for (auto& bullet : bullets)
        {
            bullet.Update();
        }

        // Remove inactive bullets (off screen) from bullets vector
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [](const Bullet& b) { return !b.IsActive(); }),
            bullets.end()
        );

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw Ship
        ship.Draw();

        // Draw Bullets
        for (auto& bullet : bullets)
        {
            bullet.Draw();
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadSound(level_start);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
