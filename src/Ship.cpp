#include "../include/Ship.h"

#include <iostream>

Ship::Ship(Texture2D shipTexture[], Vector2 pos, Color color, float speed) : isDied(false)
{
    for (int i = 0; i < NUM_SHIP_FRAME; ++i)
    {
        this->shipTexture[i] = shipTexture[i];
    }
    this->pos = pos;
    this->speed = speed;
    this->color = color;
}

void Ship::Update()
{
    if (isDied == false)
    {
        if (IsKeyDown(KEY_LEFT))
        {
            this->pos.x -= speed;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            this->pos.x += speed;
        }

        if (pos.x <= 0)
        {
            pos.x = 0;
        }
        else if (shipTexture[NUM_SHIP_FRAME - 1].width + pos.x >= GetScreenWidth())
        {
            pos.x = GetScreenWidth() - shipTexture[NUM_SHIP_FRAME - 1].width;
        }
    }
}


void Ship::Draw()
{
    if (isDied == false)
    {
        if (IsKeyDown(KEY_LEFT))
        {
            DrawTexture(shipTexture[0], this->pos.x, this->pos.y, WHITE); // ship left animation
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            DrawTexture(shipTexture[2], this->pos.x, this->pos.y, WHITE); // ship right animation
        }
        else
        {
            DrawTexture(shipTexture[NUM_SHIP_FRAME / 2], this->pos.x, this->pos.y, WHITE); //Idle animation
        }
    }
}

Vector2 Ship::GetPos() const
{
    return pos;
}

void Ship::CheckCollision(const Rectangle& alienBulletRect)
{
    // Get the bounding box of the player's ship
    Rectangle shipRect = {
        pos.x, pos.y, static_cast<float>(shipTexture[1].width / 6), static_cast<float>(shipTexture[1].height - 50)
    };

    // Check for AABB collision
    if (shipRect.x < alienBulletRect.x + alienBulletRect.width &&
        shipRect.x + shipRect.width > alienBulletRect.x &&
        shipRect.y < alienBulletRect.y + alienBulletRect.height &&
        shipRect.y + shipRect.height > alienBulletRect.y &&
        !isDied)
    {
        std::cout << "Player Collision!" << std::endl;
        isDied = true;
    }
}
