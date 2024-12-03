#pragma once
#include <raylib.h>
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"


class Game
{
private:
    void DeleteInactiveLasers();
    std::vector<Obstacle> CreateObstacles();
    std::vector<Alien> CreateAliens();
    void MoveAliens();
    void MoveDownAliens(int distance);
    void AlienShootLaser();
    std::vector<Obstacle> obstacles;
    std::vector<Alien> aliens;
    Spaceship spaceship;
    int alienDirection;
    std::vector<Laser> alienLasers;
    constexpr  static float aleinLaserShootInterval = 0.35;
    float timeLastAlienFired;
public:
    Game();
    ~Game();
    void Draw();
    void Update();
    void HandleInput();
};

