#ifndef SPACE_INVADERS_ALIEN_H
#define SPACE_INVADERS_ALIEN_H
#include "raylib.h"


class Alien
{
public:
    Alien(Texture2D alienTexture,Vector2 position, int row, int col);
    void Draw() const;
    void Update(int columCount);
    void PlayMovementSound() const;
    void UnloadMovementSound() const;

private:
    Texture2D alien_texture;
    Vector2 position;
    int width, height;
    float speed;
    int paddingX, paddingY;
    int row, col;
    float const MAX_MOVEMENT_DELAY = 1;
    float movement_elapsed_time;
    float delta_time;
    Sound movement_sound;
};


#endif //SPACE_INVADERS_ALIEN_H
