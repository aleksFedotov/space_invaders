#include "alien.hpp"

// Static array to store alien textures for each type and animation frame
Texture2D Alien::alienImages[3][2];

// Constructor to initialize an alien of a specific type at a given position
Alien::Alien(int type, Vector2 position)
{
    this->type = type; // Alien type (e.g., 1, 2, or 3)
    this->position = position; // Initial position of the alien
    frame = 0; // Current animation frame
    timeSinceLastFrame = 0.0; // Time elapsed since the last frame change

    // Load textures only once for each alien type
    if (alienImages[type - 1][frame].id == 0) 
    {
        switch (type)
        {
        case 1: // Alien type 1
            alienImages[0][0] = LoadTexture("Graphics/enemy1-1.png"); // Frame 1
            alienImages[0][1] = LoadTexture("Graphics/enemy1-2.png"); // Frame 2
            break;
        case 2: // Alien type 2
            alienImages[1][0] = LoadTexture("Graphics/enemy2-1.png"); // Frame 1
            alienImages[1][1] = LoadTexture("Graphics/enemy2-2.png"); // Frame 2
            break;
        case 3: // Alien type 3
            alienImages[2][0] = LoadTexture("Graphics/enemy3-1.png"); // Frame 1
            alienImages[2][1] = LoadTexture("Graphics/enemy3-2.png"); // Frame 2
            break;
        default:
            break;
        }
    }

    // Set alien dimensions based on the loaded texture
    alienWidth = (float)alienImages[type - 1][0].width;
    alienHeight = (float)alienImages[type - 1][0].height;

    // Scale dimensions for rendering
    alienWidthScaled = alienWidth * 3;
    alienHeightScaled = alienHeight * 3;
}

// Updates the alien's position and animation frame
void Alien::Update(int direction, float deltaTime)
{
    // Move the alien horizontally based on the direction
    position.x += direction;

    // Update the animation timer
    timeSinceLastFrame += deltaTime;

    // Switch animation frame if enough time has elapsed
    constexpr float frameInterval = 0.5; // Time interval between frames
    if (timeSinceLastFrame >= frameInterval)
    {
        frame = (frame + 1) % 2; // Toggle between frame 0 and frame 1
        timeSinceLastFrame = 0.0; // Reset the timer
    }
}

// Draws the alien at its current position with the appropriate frame
void Alien::Draw()
{
    // Define the source rectangle from the texture
    Rectangle source = { 0, 0, alienWidth, alienHeight };

    // Define the destination rectangle for rendering (scaled size)
    Rectangle dest = { position.x, position.y, alienWidthScaled, alienHeightScaled };

    // No offset or rotation for the origin
    Vector2 origin = { 0, 0 };

    // Draw the alien with the current animation frame
    DrawTexturePro(alienImages[type - 1][frame], source, dest, origin, 0.0, WHITE);
}

// Returns the alien's type
int Alien::GetType()
{
    return type;
}

// Unloads the textures for all alien types to free memory
void Alien::UnloadImages()
{
    for (int i = 0; i < 3; i++) // Iterate over the three alien types
    {
        UnloadTexture(alienImages[i][0]); // Unload frame 1
        UnloadTexture(alienImages[i][1]); // Unload frame 2
    }
}

// Returns the hitbox (rectangle) for collision detection
Rectangle Alien::getHitBox()
{
    return { position.x, position.y, alienWidthScaled, alienHeightScaled };
}
