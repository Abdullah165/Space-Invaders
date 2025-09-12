#include "../include/Bullet.h"

Bullet::Bullet(Vector2 position, float speed, Texture2D buletTexture, int direction)
    : position(position), speed(speed), bulletTexture(buletTexture), direction(direction), active(true)
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

void Bullet::SetActive(bool active)
{
    this->active = active;
}

bool Bullet::IsActive() const
{
    return active;
}
