//
// Created by Lenovo on 9/2/2025.
//

#ifndef SPACE_INVADERS_SHIP_H
#define SPACE_INVADERS_SHIP_H
#include "raylib.h"

#define NUM_SHIP_FRAME 3
class Ship
{
public:
    Ship(Texture2D shipTexture[], Vector2 pos, Color color, float speed);

    void Update();

    void Draw() ;

    Vector2 GetPos() const;

    void CheckCollision(const Rectangle& aliensBulletRec);

private:
    Texture2D shipTexture[NUM_SHIP_FRAME];
    Vector2 pos{};
    Color color{};
    float speed;
    bool isDied;
};

#endif //SPACE_INVADERS_SHIP_H
