#pragma once
#include <raylib.h>
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"


class Game
{
private:
    void DeleteInactiveLasers();
    std::vector<Obstacle> CreateObstacles();
    std::vector<Alien> CreateAliens();
    void MoveAliens();
    void MoveDownAliens(int distance);
    void AlienShootLaser();
    void CheckForCollisions();
    void GameOver();
    void Reset();
    void InitGame();
    void checkForHighScore();
    void saveHighScoreToFile(int highScore);
    int loadHighScoreFromFile();
    void UpdateAlienSpeed();
    void LevelUp();
    std::vector<Obstacle> obstacles;
    std::vector<Alien> aliens;
    Spaceship spaceship;
    int alienDirection;
    std::vector<Laser> alienLasers;
    constexpr  static float aleinLaserShootInterval = 0.35;
    float timeLastAlienFired;
    MysteryShip mysteryShip;
    float mysteryShipSpawnInterval;
    float lastTimeSpawn;
    Sound explosionSound;
    float alienSpeed;
public:
    Game();
    ~Game();
    void Draw();
    void Update();
    void HandleInput();
    bool run;
    int lives;
    int score;
    int highScore;
    int level;
    Music music;
};

