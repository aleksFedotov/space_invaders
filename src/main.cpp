#include <raylib.h>
#include "colors.hpp"
#include "game.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include <iostream>


int main() 
{   

    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;

    InitWindow(windowWidth +offset, windowHeight + 2 * offset, "C++ Space Invaders");
    Font font = LoadFontEx("Font/monogram.ttf", 64,0,0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");
    SetTargetFPS(60);
    Game game;


    
   

    while(!WindowShouldClose()) {
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
            // std::cout << x << std::endl;
            DrawTextureV(spaceshipImage, {x,745}, WHITE);
        }
        game.Draw();
        
        EndDrawing();
    }

   CloseWindow();
}