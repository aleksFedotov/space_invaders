#include "laser.hpp"
#include "colors.hpp"

// Constructor that initializes a Laser object with its position and speed
Laser::Laser(Vector2 position, int speed)
{
    this -> position = position;  // Set the position of the laser
    this -> speed = speed;        // Set the speed of the laser
    active = true;                // Initially, the laser is active
}

// Function to draw the laser on the screen
void Laser::Draw() 
{   
    if(active)  // Only draw if the laser is active
    {
        // Draw a yellow rectangle representing the laser
        DrawRectangle(position.x, position.y, 4, 15, yellow);
    }
}

// Function to get the hitbox of the laser, used for collision detection
Rectangle Laser::getHitBox()
{   
    Rectangle rect;  // Declare a rectangle to represent the hitbox
    rect.x = position.x;  // Set the x position of the hitbox
    rect.y = position.y;  // Set the y position of the hitbox
    rect.width = 4;       // Set the width of the hitbox (same as the laser)
    rect.height = 15;     // Set the height of the hitbox (same as the laser)
    return rect;          // Return the hitbox
}

// Function to update the laser's position based on its speed and deactivate if out of bounds
void Laser::Update() 
{
    position.y += speed;  // Update the position of the laser, moving it up or down based on speed

    if(active)  // Only check if the laser is active
    {
        // Deactivate the laser if it goes off the screen (above or below the play area)
        if(position.y > GetScreenHeight() - 25 || position.y < 25) 
        {
            active = false;  // Deactivate the laser if it's out of screen bounds
        } 
    }
}