#include "../include/Ship.h"

Ship::Ship(Vector2 pos, int width, int height, Color color, float speed)
{
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


void Ship::Draw() const
{
    DrawRectangle(this->pos.x, this->pos.y, this->width, this->height, this->color);
}

Vector2 Ship::GetPos() const
{
    return pos;
}
