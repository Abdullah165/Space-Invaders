#include <iostream>
#include <raylib.h>

#include "../include/Ship.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    auto ship = Ship(Vector2(screenWidth / 2.2, screenHeight  * 0.01), 80, 80,WHITE, 10);

    InitWindow(screenWidth, screenHeight, "Space Invaders");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ship.Update();

        ClearBackground(BLACK);

        ship.Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
