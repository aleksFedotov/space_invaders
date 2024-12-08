#include <raylib.h>
#include "colors.hpp"
#include "game.hpp"
#include <string>
#include <iostream>

std::string FormatWithLeadingZeros(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZero = width - numberText.length();
    numberText = std::string(leadingZero, '0') + numberText;

    return numberText;
}


int main() 
{   

    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;

    InitWindow(windowWidth +offset, windowHeight + 2 * offset, "C++ Space Invaders");
    InitAudioDevice();

    Font font = LoadFontEx("Font/space_invaders.ttf", 64,0,0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    SetTargetFPS(60);

    Game game;


    
    Texture2D background = LoadTexture("Graphics/SpaceInvaders_Background.png");


   
    Texture2D backgroundBuildings = LoadTexture("Graphics/SpaceInvaders_BackgroundBuildings.png");

    int backgroundBuildingsWidth = backgroundBuildings.width * 3;
    int backgroundBuildingsHeight = backgroundBuildings.height * 3;

    Texture2D BackgroundFloor = LoadTexture("Graphics/SpaceInvaders_BackgroundFloor.png");


    while(!WindowShouldClose()) {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        // Background Star Sky 
        for (int x = 0; x < windowWidth + background.width; x += background.width)
        {
            for (int y = 0; y < windowHeight; y += background.height)
            {   
        
                DrawTexture(background, x, y, WHITE);
                
            }
        }
        // Background Buildings 
        for (int x = 0; x < windowWidth + backgroundBuildingsWidth; x += backgroundBuildingsWidth)
        {
            Rectangle source = { 0, 0, (float)backgroundBuildings.width, (float)backgroundBuildings.height };  
            Rectangle dest = { (float)x, 350.0, (float)backgroundBuildingsWidth, (float)backgroundBuildingsHeight };  
            Vector2 origin = { 0, 0 };  
            DrawTexturePro(backgroundBuildings, source, dest, origin, 0.0f, WHITE);
        }
        // Background DArk Floor 
        for (int x = 0; x < windowWidth + BackgroundFloor.width; x += BackgroundFloor.width)
        {
            for (int y = 350 + backgroundBuildingsHeight; y < windowHeight + BackgroundFloor.height *2; y += BackgroundFloor.height)
            {   
                
                DrawTexture(BackgroundFloor, x, y, WHITE);
                
            }
        }
        DrawRectangleRoundedLines({10,10,780,780}, 0.18 ,20,yellow);
        DrawLineEx({25,730}, {775,730}, 3, yellow);
        if(game.run) 
        {
            std::string levelText = FormatWithLeadingZeros(game.level, 2);
            DrawTextEx(font, ("LEVEL " + levelText).c_str(), {570, 740},34,2,yellow);
        } else 
        {
            DrawTextEx(font, "GAME OVER", {570, 740},34,2,yellow);
        }
        
        for(int i = 0 ; i < game.lives; i++) 
        {
            float x = 50.0 * (i + 1);
            
            DrawTextureV(spaceshipImage, {x,745}, WHITE);
        }

        DrawTextEx(font, "SCORE",{50,25},34,2,yellow);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(),{50,50},34,2,yellow);

        DrawTextEx(font, "HIGH-SCORE", {550, 25}, 34, 2, yellow);
        std::string highscoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highscoreText.c_str(), {640, 50}, 34, 2, yellow);
        

        
        game.Draw();
        
        EndDrawing();
    }
    UnloadTexture(background);
    UnloadTexture(backgroundBuildings);
    UnloadTexture(BackgroundFloor);
    CloseWindow();
    CloseAudioDevice();
}