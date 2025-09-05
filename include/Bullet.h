#ifndef SPACE_INVADERS_BULLET_H
#define SPACE_INVADERS_BULLET_H
#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector2 position, float speed, Texture2D bulletTexture);

    void Draw() const;
    void Update();

    bool IsActive() const;

private:
    Texture2D bulletTexture;
    Vector2 position;
    float speed;
    bool active;
};


#endif //SPACE_INVADERS_BULLET_H
