#include "game.hpp"
#include <iostream>
#include <vector>
#include <fstream>

// Constructor of the Game class
Game::Game()
{
    // Load the background music and explosion sound
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.wav");

    // Play the background music
    PlayMusicStream(music);

    // Initialize the game elements (obstacles, aliens, etc.)
    InitGame();
}

// Destructor of the Game class
Game::~Game() 
{
    // Unload the alien images and game assets when the game ends
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

// Function to delete inactive lasers (both spaceship and alien lasers)
void Game::DeleteInactiveLasers()
{
    // Remove inactive spaceship lasers
    for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) 
    {
        if(!it->active)
        {
            it = spaceship.lasers.erase(it); // Erase inactive laser
        } 
        else 
        {
            ++it;
        }
    }

    // Remove inactive alien lasers
    for(auto it = alienLasers.begin(); it != alienLasers.end();) 
    {
        if(!it->active)
        {
            it = alienLasers.erase(it); // Erase inactive laser
        } 
        else 
        {
            ++it;
        }
    }
}

// Function to create obstacles in the game
std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3; // Width of each obstacle
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5; // Calculate gap between obstacles

    // Create 4 obstacles and position them
    for(int i = 0; i < 4; i++) 
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 200)}));
    }

    return obstacles;
}

// Function to create aliens and place them in a grid formation
std::vector<Alien> Game::CreateAliens()
{   
    std::vector<Alien> aliens;

    // Create 5 rows and 11 columns of aliens
    for(int row = 0; row < 5; row++) 
    {
        for(int col = 0; col < 11; col++) 
        {   
            // Set alien type based on the row
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
            
            // Calculate the position for each alien
            float x = 75 + col * 55;
            float y = 110 + row * 55;

            // Add the alien to the vector
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }

    return aliens;
}

// Function to update the alien speed based on the game level and remaining aliens
void Game::UpdateAlienSpeed()
{
    // Define base speed and increments for level and alien count
    float baseSpeed = 1.0;
    float levelIncrement = 0.1;
    float alienSpeedIncrement = 0.05;

    int remainingAliens = aliens.size();
    int totalAliens = 55;

    // Calculate alien speed
    alienSpeed = baseSpeed
                 + (level - 1) * levelIncrement
                 + (float(totalAliens - remainingAliens) / totalAliens) * alienSpeedIncrement;
}

// Function to handle leveling up the game
void Game::LevelUp()
{
    // Reset aliens, update score, and increment level
    aliens = CreateAliens();
    score += level * 500;
    score += lives * 200;

    float timeElapsed = GetTime() - waveStartTime;
    int waveBonus = std::max(0, 1000 - int(timeElapsed * 50)); // Time-based wave bonus
    score += waveBonus;

    waveStartTime = GetTime();
    level++;  // Increase the level
}

// Function to move aliens across the screen
void Game::MoveAliens()
{
    for(auto& alien : aliens) {
        // Check if aliens hit the screen boundaries
        if(alien.position.x + alien.alienWidthScaled > GetScreenWidth() - 25 ) 
        {
            alienDirection = -1; // Reverse direction
            MoveDownAliens(4);   // Move down
        }
        if(alien.position.x < 25) {
            alienDirection = 1;  // Reverse direction
            MoveDownAliens(4);   // Move down
        }

        // Update alien movement
        alien.Update(alienDirection * alienSpeed, GetFrameTime());
    }
}

// Function to move all aliens down by a certain distance
void Game::MoveDownAliens(int distance)
{
    for(auto& alien : aliens) 
    {
        alien.position.y += distance;
    }
}

// Function for aliens to shoot lasers at the spaceship
void Game::AlienShootLaser()
{
    double currentTime = GetTime();

    // Ensure aliens shoot at regular intervals
    if(currentTime - timeLastAlienFired >= aleinLaserShootInterval && !aliens.empty()) 
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];

        // Create a new alien laser and add it to the laser list
        alienLasers.push_back(Laser({alien.position.x + alien.alienWidthScaled / 2,
                                     alien.position.y + alien.alienHeightScaled }, 6));
        timeLastAlienFired = GetTime();
    }
}

// Function to check for collisions between different game objects
void Game::CheckForCollisions()
{
    // Spaceship lasers vs Aliens
    for(auto& laser : spaceship.lasers)
    {
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            // Check if laser hits an alien
            if(CheckCollisionRecs(it->getHitBox(), laser.getHitBox()))
            {
                PlaySound(explosionSound); // Play sound on hit

                // Increase score based on alien type
                if(it->type == 1) score += 10;
                else if(it->type == 2) score += 20;
                else if(it->type == 3) score += 30;

                checkForHighScore(); // Check if the score is a new high score
                it = aliens.erase(it); // Remove alien
                laser.active = false;  // Deactivate laser
                UpdateAlienSpeed();    // Update alien speed
            }
            else 
            {
                ++it;
            }
        }

        // If all aliens are destroyed, level up
        if(aliens.size() == 0) 
        {
            LevelUp();
        }

        // Check collisions with obstacles
        for(auto& obstacle : obstacles) 
        {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it->getHtBox(), laser.getHitBox()))
                {
                    it = obstacle.blocks.erase(it); // Destroy block
                    laser.active = false;  // Deactivate laser
                }
                else 
                {
                    ++it;
                }
            }
        }

        // Check if laser hits the mystery ship
        if(CheckCollisionRecs(mysteryShip.getHitBox(), laser.getHitBox()))
        {
            mysteryShip.alive = false;
            laser.active = false;
            score += 200; // Add score for destroying mystery ship
            checkForHighScore(); // Check if it is a new high score
            PlaySound(explosionSound); // Play explosion sound
        }
    }

    // Alien lasers vs Spaceship
    for(auto& laser : alienLasers)
    {
        // Check if alien laser hits spaceship
        if(CheckCollisionRecs(spaceship.getHitBox(), laser.getHitBox()))
        {
            laser.active = false;  // Deactivate laser
            lives--;               // Decrease lives
            if(lives == 0) 
            {
                GameOver(); // End game if no lives left
            }
        }

        // Check collisions with obstacles
        for(auto& obstacle : obstacles) 
        {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it->getHtBox(), laser.getHitBox()))
                {
                    it = obstacle.blocks.erase(it); // Destroy block
                    laser.active = false;  // Deactivate laser
                }
                else 
                {
                    ++it;
                }
            }
        }
    }

    // Check collisions between aliens and obstacles
    for(auto& alien : aliens)
    {
        for(auto& obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it->getHtBox(), alien.getHitBox()))
                {
                    it = obstacle.blocks.erase(it); // Destroy block
                }
                else 
                {
                    ++it;
                }
            }
        }

        // Check if alien hits spaceship
        if(CheckCollisionRecs(alien.getHitBox(), spaceship.getHitBox()))
        {
            GameOver(); // End game if alien collides with spaceship
        }
    }
}

// Function to end the game
void Game::GameOver()
{
    run = false;  // Stop the game loop
}

// Initialize the game settings, such as obstacles, aliens, score, etc.
void Game::InitGame()
{
    obstacles = CreateObstacles();  // Create obstacles
    aliens = CreateAliens();        // Create aliens
    alienDirection = 1;             // Set initial alien direction
    timeLastAlienFired = 0;         // Time of last alien laser shot
    lastTimeSpawn = 0;              // Time of last mystery ship spawn
    mysteryShipSpawnInterval = GetRandomValue(10, 20);  // Set spawn interval for mystery ship
    lives = 3;                      // Set initial lives
    run = true;                     // Start the game loop
    score = 0;                      // Initialize score
    highScore = loadHighScoreFromFile(); // Load high score from file
    level = 1;                      // Start at level 1
    alienSpeed = 1;                 // Set initial alien speed
    waveStartTime = GetTime();      // Set wave start time
}

// Check if the current score is a new high score
void Game::checkForHighScore()
{
    if(score > highScore)
    {
        highScore = score;  // Update high score
        saveHighScoreToFile(highScore);  // Save the new high score
    }
}

// Save the high score to a file
void Game::saveHighScoreToFile(int highScore)
{
    std::ofstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open())
    {
        highScoreFile << highScore; // Write the high score to the file
        highScoreFile.close();
    } else 
    {
        std::cerr << "Failed to save highscore to file" << std::endl; // Error handling
    }
}

// Load the high score from a file
int Game::loadHighScoreFromFile()
{   
    int loadedHighScore = 0;
    std::ifstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open())
    {
        highScoreFile >> loadedHighScore; // Read the high score from the file
        highScoreFile.close();
    } else 
    {
        std::cerr << "Failed to load highscore from file" << std::endl; // Error handling
    }
    return loadedHighScore;
}

// Reset the game to its initial state
void Game::Reset() 
{
    spaceship.Reset();  // Reset spaceship position and state
    aliens.clear();     // Clear aliens
    alienLasers.clear(); // Clear alien lasers
    obstacles.clear();  // Clear obstacles
}

// Update the game state (movement, spawning, collisions, etc.)
void Game::Update() 
{
    if(run) {
        double currentTime = GetTime();
        
        // Spawn mystery ship at random intervals
        if(currentTime - lastTimeSpawn > mysteryShipSpawnInterval) 
        {
            mysteryShip.Spawn();  // Spawn the mystery ship
            lastTimeSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);  // Randomize next spawn interval
        }

        // Update spaceship lasers
        for(auto& laser : spaceship.lasers) 
        {
            laser.Update();
        }

        MoveAliens();       // Move the aliens
        AlienShootLaser();  // Let aliens shoot lasers

        // Update alien lasers
        for(auto& laser : alienLasers) 
        {
            laser.Update();
        }

        DeleteInactiveLasers(); // Remove inactive lasers
        mysteryShip.Update();  // Update mystery ship

        CheckForCollisions();  // Check all collisions
    } else {
        // Reset game when the player presses Enter
        if(IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        }
    }
}

// Draw all the game objects to the screen
void Game::Draw() 
{
    spaceship.Draw();  // Draw spaceship
    for(auto& laser :spaceship.lasers) 
    {
        laser.Draw();  // Draw spaceship lasers
    }

    for(auto& obstacle : obstacles) 
    {
        obstacle.Draw();  // Draw obstacles
    }

    for(auto& alien : aliens) 
    {
        alien.Draw();  // Draw aliens
    }

    for(auto& laser : alienLasers) 
    {
        laser.Draw();  // Draw alien lasers
    }

    mysteryShip.Draw();  // Draw mystery ship
}

// Handle player input (movement and firing lasers)
void Game::HandleInput() 
{
    if(run)
    {
        if(IsKeyDown(KEY_LEFT)) 
        {
            spaceship.MoveLeft();  // Move spaceship left
        } 
        else if (IsKeyDown(KEY_RIGHT))
        {
            spaceship.MoveRight();  // Move spaceship right
        } else if(IsKeyDown(KEY_SPACE))
        {
            spaceship.FireLaser();  // Fire laser from spaceship
        }
    }
}
