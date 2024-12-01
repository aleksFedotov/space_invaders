#include <raylib.h>
#include "colors.h"
#include "spaceship.h"


int main() 
{
    int windowWidth = 750;
    int windowHeight = 700;

    InitWindow(windowWidth, windowHeight, "C++ Space Invaders");
    SetTargetFPS(60);
    Spaceship spaceship;

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(grey);
        spaceship.Draw();

        EndDrawing();
    }

   CloseWindow();
}