#include "../include/Bullet.h"

Bullet::Bullet(Vector2 position, float speed, Texture2D buuletTexture, int direction)
    : position(position), speed(speed), bulletTexture(buuletTexture), direction(direction), active(true)
{
}

void Bullet::Draw() const
{
    if (active)
    {
        DrawTexture(bulletTexture, position.x, position.y,WHITE);
    }
}

void Bullet::Update()
{
    if (active)
    {
        position.y += speed * direction;
        if (position.y < 0 || position.y >= GetScreenHeight())
        {
            active = false;
        }
    }
}

Vector2 Bullet::GetPosition() const
{
    return position;
}

float Bullet::GetWidth() const
{
    return bulletTexture.width;
}

float Bullet::GetHeight() const
{
    return bulletTexture.height;
}


bool Bullet::IsActive() const
{
    return active;
}
