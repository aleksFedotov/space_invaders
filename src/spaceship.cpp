#include "spaceship.hpp"

Spaceship::Spaceship()
{
    // Load the spaceship texture from a file
    image = LoadTexture("Graphics/spaceship.png");
    
    // Position the spaceship at the center of the screen, with a 100-pixel offset from the bottom
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;

    // Initialize the time for the last laser fire
    lastFireTime = 0.0;

    // Load the laser firing sound effect
    laserSound = LoadSound("Sounds/shoot.wav");
}

Spaceship::~Spaceship()
{
    // Unload the texture and sound when the spaceship is destroyed
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void Spaceship::Draw() 
{
    // Draw the spaceship texture at its current position
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft()
{
    // Move the spaceship to the left by 7 pixels
    position.x -= 7;
    
    // Prevent the spaceship from going off the left edge of the screen
    if (position.x < 25) 
    {
        position.x = 25;
    }
}

void Spaceship::MoveRight()
{
    // Move the spaceship to the right by 7 pixels
    position.x += 7;
    
    // Prevent the spaceship from going off the right edge of the screen
    if (position.x > GetScreenWidth() - image.width - 25) 
    {
        position.x = GetScreenWidth() - image.width - 25;
    }
}

void Spaceship::FireLaser()
{   
    // Check if enough time has passed since the last laser was fired (at least 0.35 seconds)
    if (GetTime() - lastFireTime >= 0.35)
    {
        // Create a new laser at the center of the spaceship and move it upwards (-6 speed)
        lasers.push_back(Laser({position.x + image.width / 2 - 2, position.y}, -6));
        
        // Update the last fire time to prevent rapid firing
        lastFireTime = GetTime();
        
        // Play the laser shooting sound
        PlaySound(laserSound);
    }
}

Rectangle Spaceship::getHitBox()
{  
    // Return the hitbox of the spaceship, which is its rectangle bounds
    return {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset()
{
    // Reset the spaceship position to the center of the screen with the same offset from the bottom
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;
    
    // Clear the lasers vector (i.e., remove all lasers)
    lasers.clear();
}
