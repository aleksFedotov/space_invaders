#pragma once
#include <raylib.h>
#include "spaceship.hpp"
#include "obstacle.hpp"


class Game
{
private:
    void DeleteInactiveLasers();
    std::vector<Obstacle> CreateObstacles();
    std::vector<Obstacle> obstacles;
    Spaceship spaceship;
public:
    Game();
    ~Game();
    void Draw();
    void Update();
    void HandleInput();
};

