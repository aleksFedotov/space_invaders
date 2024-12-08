#include "game.hpp"
#include <iostream>
#include <vector>
#include <fstream>


Game::Game()
{
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.wav");
    PlayMusicStream(music);
    InitGame();
}

Game::~Game() 
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::DeleteInactiveLasers()
{
    for(auto  it = spaceship.lasers.begin(); it != spaceship.lasers.end();) 
    {
        if(!it -> active )
        {
            it = spaceship.lasers.erase(it);
        } else 
        {
            ++ it;
        }
    }

    for(auto  it = alienLasers.begin(); it != alienLasers.end();) 
    {
        if(!it -> active )
        {
            it = alienLasers.erase(it);
        } else 
        {
            ++ it;
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;


    for(int i = 0; i < 4; i++) 
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() -200)}));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{   
    std::vector<Alien> aliens;

    for(int row = 0; row < 5; row++) 
    {
        for(int col = 0; col < 11; col++) 
        {   
            int alienType;
            if(row == 0)
            {
                alienType = 3;
            }
            else if(row == 1 || row == 2) 
            {
                alienType = 2;
            } 
            else 
            {
                alienType = 1;
            }
            float  x = 75 + col * 55;
            float  y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x,y}));
        }
    }
    return aliens;
}

void Game::UpdateAlienSpeed()
{
    float baseSpeed = 1.0;                
    float levelIncrement = 0.1;           
    float alienSpeedIncrement = 0.05;     

    int remainingAliens = aliens.size();
    int totalAliens = 55;                  

    alienSpeed = baseSpeed
                 + (level - 1) * levelIncrement
                 + (float(totalAliens - remainingAliens) / totalAliens) * alienSpeedIncrement;
//    std::cout << alienSpeed << std::endl;
}

void Game::LevelUp()
{
    
    aliens = CreateAliens();
    score += level * 500;
    score += lives * 200;

    float timeElapsed = GetTime() - waveStartTime;
    int waveBonus = std::max(0, 1000 - int(timeElapsed * 50));
    score += waveBonus;
    waveStartTime = GetTime();
    level++;
    
}

void Game::MoveAliens()
{
    for(auto& alien : aliens) {
        if(alien.position.x + alien.alienWidthScaled > GetScreenWidth() - 25 ) 
        {
            alienDirection = -1;
            MoveDownAliens(4);
            
        }
        if(alien.position.x < 25) {
            alienDirection = 1;
            MoveDownAliens(4);

           
        }
        alien.Update(alienDirection * alienSpeed, GetFrameTime());
        
    }

}

void Game::MoveDownAliens(int distance)
{
    for(auto& alien : aliens) 
    {
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired >= aleinLaserShootInterval && !aliens.empty()) 
    {
        int randomIndex = GetRandomValue(0, aliens.size() -1);

        Alien& alien = aliens[randomIndex];

        alienLasers.push_back(Laser({alien.position.x + alien.alienWidthScaled/2,
                                    alien.position.y - alien.alienHeightScaled },6));
        timeLastAlienFired = GetTime();
    }
}

void Game::CheckForCollisions()
{
    // Spaceship Lasers
    for(auto& laser : spaceship.lasers)
    {
       
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            if(CheckCollisionRecs(it -> getHitBox(), laser.getHitBox()))
            {
                PlaySound(explosionSound);
                if(it -> type == 1)
                {
                    score += 10;
                } else if(it -> type == 2)
                {
                    score += 20;
                } else if(it -> type == 3) 
                {
                    score += 30;
                }
                checkForHighScore();
                it = aliens.erase(it);
                laser.active = false;
                UpdateAlienSpeed();
                

            }
            else 
            {
                ++it;
            }
        }

        if(aliens.size() == 0) 
            {
                LevelUp();
            }

        for(auto& obstacle : obstacles) 
        {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it -> getHtBox(), laser.getHitBox()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else 
                {
                    ++it;
                }
            }
        }

        if(CheckCollisionRecs(mysteryShip.getHitBox(), laser.getHitBox()))
        {
            mysteryShip.alive = false;
            laser.active =false;
            score += 200;
            checkForHighScore();
            PlaySound(explosionSound);

        }
        
    }

    // Alien Lasers
    for(auto& laser : alienLasers)
    {
        if(CheckCollisionRecs(spaceship.getHitBox(), laser.getHitBox()))
        {
            laser.active = false;
            lives--;
            if(lives == 0)
            {
                GameOver();
            }
        }

        for(auto& obstacle : obstacles) 
        {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it -> getHtBox(), laser.getHitBox()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else 
                {
                    ++it;
                }
            }
        }
        
    }

    // Alien Collision with obstacle

    for(auto& alien : aliens)
    {
        for(auto& obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it -> getHtBox(), alien.getHitBox()))
                {
                    it = obstacle.blocks.erase(it);
                }
                else 
                {
                    ++it;
                }
            }
            
        }

        if(CheckCollisionRecs(alien.getHitBox(), spaceship.getHitBox()))
        {
            GameOver();
        }
    }
}

void Game::GameOver()
{
    run = false;
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    alienDirection = 1;
    timeLastAlienFired = 0;
    lastTimeSpawn = 0;
    mysteryShipSpawnInterval = GetRandomValue(10,20);
    lives = 3;
    run = true;
    score = 0;
    highScore = loadHighScoreFromFile();
    level = 1;
    alienSpeed = 1;
    waveStartTime = GetTime();

}

void Game::checkForHighScore()
{
    if(score > highScore)
    {
        highScore = score;
        saveHighScoreToFile(highScore);
    }
}

void Game::saveHighScoreToFile(int highScore)
{
    std::ofstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open())
    {
        highScoreFile << highScore;
        highScoreFile.close();
    } else 
    {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::loadHighScoreFromFile()
{   
    int loadedHighSCore = 0;
    std::ifstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open())
    {
        highScoreFile >> loadedHighSCore;
        highScoreFile.close();
    } else 
    {
        std::cerr << "Failed to load highscore t file" << std::endl;
    }
    return loadedHighSCore;
}

void Game::Reset() 
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

void Game::Update() 
{
    if(run) {
        double currentTime = GetTime();
        if(currentTime - lastTimeSpawn > mysteryShipSpawnInterval) 
        {
            mysteryShip.Spawn();
            lastTimeSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10,20);
        }
        for(auto& laser : spaceship.lasers) 
        {
            laser.Update();
        }

        MoveAliens();
        AlienShootLaser();

        for(auto& laser : alienLasers) 
        {
            laser.Update();
        }
        DeleteInactiveLasers();
        mysteryShip.Update();

        CheckForCollisions();
    } else {
        if(IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        }
    }
   
}

void Game::Draw() 
{
    spaceship.Draw();
    for(auto& laser :spaceship.lasers) 
    {
        laser.Draw();
    }

    for(auto& obstacle : obstacles) 
    {
        obstacle.Draw();
    }

    for(auto& alien : aliens) 
    {
        alien.Draw();
    }

    for(auto& laser : alienLasers) 
    {
        laser.Draw();
    }

    mysteryShip.Draw();
}

void Game::HandleInput() 
{
    if(run)
    {
        if(IsKeyDown(KEY_LEFT)) 
        {
            spaceship.MoveLeft();
        } 
        else if (IsKeyDown(KEY_RIGHT))
        {
            spaceship.MoveRight();
        } else if(IsKeyDown(KEY_SPACE))
        {
            spaceship.FireLaser();
        }
    }
}