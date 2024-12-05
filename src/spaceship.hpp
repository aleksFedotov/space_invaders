#pragma once
#include <raylib.h>
#include "laser.hpp"
#include <vector>



class Spaceship
{
private:
    Texture2D image;
    Vector2 position;
    double lastFireTime;
public:
    Spaceship();
    ~Spaceship();
    void Draw();
    void MoveLeft();
    void MoveRight();
    void FireLaser();
    Rectangle getHitBox();
    void Reset();
    std::vector<Laser> lasers;
    Sound laserSound;
    
};


