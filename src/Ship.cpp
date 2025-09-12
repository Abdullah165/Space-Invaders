#include "../include/Ship.h"

#include <iostream>

Ship::Ship(Texture2D shipTexture[], Vector2 pos, Color color, float speed) : number_of_lives(3), isDied(false)
{
    for (int i = 0; i < NUM_SHIP_FRAME; ++i)
    {
        this->shipTexture[i] = shipTexture[i];
    }
    this->pos = pos;
    this->speed = speed;
    this->color = color;
}

void Ship::Draw()
{
    if (!HasDead())
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

void Ship::Update()
{
    if (!HasDead())
    {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            this->pos.x -= speed;
        }
        else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
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

void Ship::SetNumOfLives(int numOfLives)
{
    this->number_of_lives = numOfLives;
}


int Ship::GetNumOfLives() const
{
    return number_of_lives;
}

bool Ship::CheckCollision(const Rectangle& alienBulletRect)
{
    if (HasDead()) return false;

    // Get the bounding box of the player's ship
    Rectangle shipRect = {
        pos.x, pos.y, static_cast<float>(shipTexture->width), static_cast<float>(shipTexture->height)
    };


    // Check for AABB collision
    if (shipRect.x < alienBulletRect.x + 40 + (alienBulletRect.width * 0.05f) &&
        shipRect.x + shipRect.width > alienBulletRect.x + 40 &&
        shipRect.y < alienBulletRect.y +40 + (alienBulletRect.height * 0.2f) &&
        shipRect.y + shipRect.height > alienBulletRect.y +40 &&
        !isDied)
    {
        number_of_lives--;
        return true;
    }
    return false;
}

bool Ship::HasDead() const
{
    return number_of_lives == 0;
}

Vector2 Ship::GetPos() const
{
    return pos;
}
