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


    
   

    while(!WindowShouldClose()) {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLines({10,10,780,780}, 0.18 ,20,yellow);
        DrawLineEx({25,730}, {775,730}, 3, yellow);
        if(game.run) 
        {
            DrawTextEx(font, "LEVEL 01", {570, 740},34,2,yellow);
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

   CloseWindow();
   CloseAudioDevice();
}