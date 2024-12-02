#include <raylib.h>
#include "colors.hpp"
#include "game.hpp"
#include "obstacle.hpp"


int main() 
{
    int windowWidth = 750;
    int windowHeight = 700;

    InitWindow(windowWidth, windowHeight, "C++ Space Invaders");
    SetTargetFPS(60);
    Game game;
   

    while(!WindowShouldClose()) {
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        game.Draw();
        EndDrawing();
    }

   CloseWindow();
}