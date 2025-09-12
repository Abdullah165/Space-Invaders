#include "../include/Alien.h"

#include <iostream>


Alien::Alien(Texture2D alienTexture, Vector2 position, int row, int col) : alien_texture(alienTexture),
                                                                           position(position), row(row), col(col),
                                                                           width(50), height(50),
                                                                           paddingX(20),
                                                                           paddingY(20), speed(30),
                                                                           movement_elapsed_time(MAX_MOVEMENT_DELAY)
                                                                           , delta_time(0.02f), active(true)
{
    movement_sound = LoadSound("assets/sounds/alien-move-sfx.wav");
}

void Alien::Draw()
{
    if (!active) return;

    alienTexturePosX = position.x + col * (paddingX + width); // Add padding in x-axis
    alienTexturePosY = position.y + row * (paddingY + height); // Add padding in y-axis
    //DrawRectangle(x, y, width, height, WHITE);
    DrawTexture(alien_texture, alienTexturePosX, alienTexturePosY, WHITE);
}

void Alien::Update(int columCount)
{
    if (!active) return;

    movement_elapsed_time -= delta_time;
    if (movement_elapsed_time < 0)
    {
        if (columCount + paddingX + position.x >= GetScreenWidth() / 1.8)
        {
            position.y += speed * 1.5f; // Down the aliens a little bit
            delta_time += 0.015;
            speed *= -1;
        }
        else if (paddingX + position.x <= GetScreenWidth() / 10)
        {
            position.y -= speed * 1.5f;
            delta_time += 0.015;
            speed *= -1;
        }

        position.x += speed;
        movement_elapsed_time = MAX_MOVEMENT_DELAY;

        PlayMovementSound();
    }
}

bool Alien::CheckCollision(const Rectangle& shipBullet)
{
    Rectangle alienRect = {
        alienTexturePosX, alienTexturePosY, static_cast<float>(alien_texture.width / 1.5),
        static_cast<float>(alien_texture.height)
    };

    // Check for AABB collision
    if (alienRect.x <= shipBullet.x + shipBullet.width
        && alienRect.x + alienRect.width >= shipBullet.x
        && alienRect.y <= shipBullet.y + shipBullet.height
        && alienRect.y + alienRect.height >= shipBullet.y && active)
    {
        active = false;
        return  true;
    }
    return false;
}


float Alien::GetAlienPosX() const
{
    return alienTexturePosX;
}

float Alien::GetAlienPosY() const
{
    return alienTexturePosY;
}


void Alien::PlayMovementSound() const
{
    PlaySound(movement_sound);
}

void Alien::UnloadMovementSound() const
{
    UnloadSound(movement_sound);
}

bool Alien::IsActive() const
{
    return active;
}
