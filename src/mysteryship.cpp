#include "mysteryship.hpp"

// Constructor to initialize the mystery ship with its texture and set it as not alive initially
MysteryShip::MysteryShip()
{
    image = LoadTexture("Graphics/mystery.png");  // Load the mystery ship texture from file
    alive = false;  // Initially, the mystery ship is not alive (not on screen)
}

// Destructor to unload the mystery ship texture
MysteryShip::~MysteryShip()
{
    UnloadTexture(image);  // Unload the texture when the mystery ship object is destroyed
}

// Function to spawn the mystery ship at a random position (either left or right side of the screen)
void MysteryShip::Spawn() 
{
    position.y = 90;  // Set the vertical position of the mystery ship (fixed value)
    int side = GetRandomValue(0, 1);  // Randomly choose 0 (left side) or 1 (right side)

    if(side == 0) 
    {
        position.x = 25;  // Set position to the left side of the screen
        speed = 3;        // Move the ship to the right
    } 
    else 
    {
        position.x = GetScreenWidth() - image.width - 25;  // Set position to the right side of the screen
        speed = -3;       // Move the ship to the left
    }
    alive = true;  // Set the mystery ship as alive, meaning it is now on screen
}

// Function to get the hitbox of the mystery ship for collision detection
Rectangle MysteryShip::getHitBox()
{   
    if(alive) 
    {
        // Return the hitbox as a rectangle with the position and dimensions of the mystery ship
        return {position.x, position.y, float(image.width), float(image.height)};
    } 
    else 
    {
        // If the mystery ship is not alive, return a degenerate hitbox (empty)
        return {position.x, position.y, 0, 0};
    }
}

// Function to update the position of the mystery ship, moving it left or right based on speed
void MysteryShip::Update() 
{
    if(alive)
    {
        position.x += speed;  // Move the ship by its speed

        // If the mystery ship moves off-screen, set it as not alive
        if(position.x > GetScreenWidth() - image.width - 25 || position.x < 25)
        {
            alive = false;  // The ship goes off screen, so it is no longer alive
        }
    }
}

// Function to draw the mystery ship on the screen if it is alive
void MysteryShip::Draw() 
{
    if(alive)
    {
        // Draw the mystery ship's texture at its current position
        DrawTextureV(image, position, WHITE);
    }
}