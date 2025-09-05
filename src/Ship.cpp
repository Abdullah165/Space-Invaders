#include "../include/Ship.h"

Ship::Ship(Texture2D shipTexture[], Vector2 pos, int width, int height, Color color, float speed)
{
    for (int i = 0; i < NUM_SHIP_FRAME; ++i)
    {
        this->shipTexture[i] = shipTexture[i];
    }
    this->pos = pos;
    this->speed = speed;
    this->width = width;
    this->height = height;
    this->color = color;
}

void Ship::Update()
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
    else if (pos.x + width >= GetScreenWidth())
    {
        pos.x = GetScreenWidth() - width;
    }
}


void Ship::Draw()
{
    if (IsKeyDown(KEY_LEFT))
    {
        DrawTexture(shipTexture[0], this->pos.x, this->pos.y, WHITE);
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        DrawTexture(shipTexture[2], this->pos.x, this->pos.y, WHITE);
    }
    else
    {
        DrawTexture(shipTexture[NUM_SHIP_FRAME / 2], this->pos.x, this->pos.y, WHITE);
    }
}

Vector2 Ship::GetPos() const
{
    return pos;
}
