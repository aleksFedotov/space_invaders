#include "alien.hpp"

Texture2D Alien::alienImages[3][2];

Alien::Alien(int type, Vector2 position)
{
    this -> type = type;
    this -> position = position;
    frame =0;
    timeSinceLastFrame = 0.0;

    if(alienImages[type - 1][frame].id == 0) 
    {
        
        switch (type)
        {
        case 1:
            alienImages[0][0] = LoadTexture("Graphics/enemy1-1.png");
            alienImages[0][1] = LoadTexture("Graphics/enemy1-2.png");
            break;
        case 2:
            alienImages[1][0] = LoadTexture("Graphics/enemy2-1.png");
            alienImages[1][1] = LoadTexture("Graphics/enemy2-2.png");
            break;
        case 3:
            alienImages[2][0] = LoadTexture("Graphics/enemy3-1.png");
            alienImages[2][1] = LoadTexture("Graphics/enemy3-2.png");
            break;
        
        default:
            break;
        }
    }

    alienWidth = (float)alienImages[type-1][0].width;
    alienHeight = (float)alienImages[type-1][0].height;
    alienWidthScaled = (float)alienImages[type-1][0].width * 3;
    alienHeightScaled = (float)alienImages[type-1][0].height * 3;

}

void Alien::Update(int direction, float deltaTime)
{
    position.x += direction;
    timeSinceLastFrame += deltaTime;

    
    constexpr float frameInterval = 0.5; 
    if (timeSinceLastFrame >= frameInterval)
    {
        frame = (frame + 1) % 2; 
        timeSinceLastFrame = 0.0; 
    }
    
}

void Alien::Draw()
{   

    Rectangle source = {0, 0, alienWidth, alienHeight };  
    Rectangle dest = { position.x, position.y, alienWidthScaled, alienHeightScaled };  
    Vector2 origin = { 0, 0 };  
    DrawTexturePro(alienImages[type-1][frame], source, dest, origin, 0.0, WHITE);
    
    
}

int Alien::GetType()
{
    return type;
}

void Alien::UnloadImages()
{
    for(int i= 0; i < 4; i++)
    {
        UnloadTexture(alienImages[i][0]);
        UnloadTexture(alienImages[i][1]);
    }
}

Rectangle Alien::getHitBox()
{
    return {position.x, position.y, alienWidthScaled , alienHeightScaled };
}
