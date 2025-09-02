//
// Created by Lenovo on 9/2/2025.
//

#ifndef SPACE_INVADERS_SHIP_H
#define SPACE_INVADERS_SHIP_H
#include "raylib.h"

class Ship
{
public:
    Ship(Vector2 pos, int width, int height, Color color, float speed);

    void Update();
    void Draw() const;

private:
    Vector2 pos{};
    int width, height;
    Color color{};
    float speed;
};

#endif //SPACE_INVADERS_SHIP_H
