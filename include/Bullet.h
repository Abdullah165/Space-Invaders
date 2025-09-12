#ifndef SPACE_INVADERS_BULLET_H
#define SPACE_INVADERS_BULLET_H
#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector2 position, float speed, Texture2D bulletTexture, int dir);

    void Draw() const;
    void Update();
    Vector2 GetPosition() const;
    float GetWidth() const;
    float GetHeight() const;
    void SetActive(bool active);
    bool IsActive() const;

private:
    Texture2D bulletTexture;
    Vector2 position;
    float speed;
    bool active;
    int direction; // +1 = up (ship), -1 = down (enemy)
};


#endif //SPACE_INVADERS_BULLET_H
