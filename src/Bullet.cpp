#include "../include/Bullet.h"

Bullet::Bullet(Vector2 position, float speed) : position(position), speed(speed),active(true)
{
}

void Bullet::Draw() const
{
    if (active)
    {
        DrawRectangle(position.x, position.y, 10, 30, WHITE);
    }
}

void Bullet::Update()
{
    if (active)
    {
        position.y -= speed;
        if (position.y < 0)
        {
            active = false;
        }
    }
}

bool Bullet::IsActive() const
{
    return active;
}


