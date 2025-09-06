#include "../include/Alien.h"

Alien::Alien(Vector2 position, int row, int col) : position(position), row(row), col(col), width(50), height(50),
                                                   paddingX(20),
                                                   paddingY(20), speed(2)
{
}

void Alien::Draw() const
{
    int x = position.x + col * (paddingX + width); // Add padding in x-axis
    int y = position.y + row * (paddingY + height); // Add padding in y-axis
    DrawRectangle(x, y, width, height, WHITE);
}

void Alien::Update(int columCount)
{
    if ((width * columCount) + paddingX + position.x >= GetScreenWidth() / 1.2)
    {
        position.y += speed * 5;
        speed *= -1;
    }
    else if (paddingX + position.x <= GetScreenWidth() / 10)
    {
        speed *= -1;
    }

    position.x += speed;
}
