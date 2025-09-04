#ifndef SPACE_INVADERS_BULLET_H
#define SPACE_INVADERS_BULLET_H
#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector2 position, float speed);

    void Draw() const;
    void Update();

    bool IsActive() const;

private:
    Vector2 position;
    float speed;
    bool active;
};


#endif //SPACE_INVADERS_BULLET_H
