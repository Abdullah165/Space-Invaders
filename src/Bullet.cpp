#include "../include/Bullet.h"

Bullet::Bullet(Vector2 position, float speed,Texture2D buuletTexture) : position(position), speed(speed),bulletTexture(buuletTexture),active(true)
{
}

void Bullet::Draw() const
{
    if (active)
    {
        DrawTexture(bulletTexture,position.x,position.y,WHITE);
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

