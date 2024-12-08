#pragma once
#include <raylib.h>

class Alien
{
private:
    /* data */
public:

    Alien(int type, Vector2 position);
    void Update(int direction, float deltatime);
    void Draw();
    int GetType();
    static void UnloadImages();
    Rectangle getHitBox();
    static Texture2D alienImages[3][2];
    int type;
    Vector2 position;
    int frame;
    float timeSinceLastFrame;
    float alienHeight;
    float alienWidth;
    float alienHeightScaled;
    float alienWidthScaled;
};


