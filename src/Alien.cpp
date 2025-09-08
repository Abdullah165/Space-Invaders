#include "../include/Alien.h"

Alien::Alien(Texture2D alienTexture, Vector2 position, int row, int col) : alien_texture(alienTexture),
                                                                           position(position), row(row), col(col),
                                                                           width(50), height(50),
                                                                           paddingX(20),
                                                                           paddingY(20), speed(30),
                                                                           movement_elapsed_time(MAX_MOVEMENT_DELAY)
                                                                           , delta_time(0.01f)
{
    movement_sound = LoadSound("assets/sounds/alien-move-sfx.wav");
}

void Alien::Draw() const
{
    int x = position.x + col * (paddingX + width); // Add padding in x-axis
    int y = position.y + row * (paddingY + height); // Add padding in y-axis
    //DrawRectangle(x, y, width, height, WHITE);
    DrawTexture(alien_texture, x, y, WHITE);
}

void Alien::Update(int columCount)
{
    movement_elapsed_time -= delta_time;
    if (movement_elapsed_time < 0)
    {
        if (columCount + paddingX + position.x >= GetScreenWidth() / 1.8)
        {
            position.y += speed * 2; // Down the aliens a little bit
            delta_time += 0.02;
            speed *= -1;
        }
        else if (paddingX + position.x <= GetScreenWidth() / 10)
        {
            speed *= -1;
        }

        position.x += speed;
        movement_elapsed_time = MAX_MOVEMENT_DELAY;

        PlayMovementSound();
    }
}

void Alien::PlayMovementSound() const
{
    PlaySound(movement_sound);
}

void Alien::UnloadMovementSound() const
{
    UnloadSound(movement_sound);
}
