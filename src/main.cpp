#include <raylib.h>
#include "colors.hpp"
#include "game.hpp"
#include <string>
#include <iostream>

// Function to format a number with leading zeros up to the specified width
std::string FormatWithLeadingZeros(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZero = width - numberText.length();
    numberText = std::string(leadingZero, '0') + numberText;

    return numberText;
}

int main() 
{   
    // Window and game dimensions
    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;

    // Initialize the game window and audio system
    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "C++ Space Invaders");
    InitAudioDevice();

    // Load custom font for UI text
    Font font = LoadFontEx("Font/space_invaders.ttf", 64, 0, 0);

    // Load texture for spaceship lives indicator
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    // Set target frames per second for smooth gameplay
    SetTargetFPS(60);

    // Initialize game instance
    Game game;

    // Load background textures for the game
    Texture2D background = LoadTexture("Graphics/SpaceInvaders_Background.png");
    Texture2D backgroundBuildings = LoadTexture("Graphics/SpaceInvaders_BackgroundBuildings.png");

    // Adjust scaling of background buildings
    int backgroundBuildingsWidth = backgroundBuildings.width * 3;
    int backgroundBuildingsHeight = backgroundBuildings.height * 3;

    // Load the texture for the floor
    Texture2D BackgroundFloor = LoadTexture("Graphics/SpaceInvaders_BackgroundFloor.png");

    // Main game loop
    while (!WindowShouldClose()) 
    {
        // Update the music stream (ensures looping music plays correctly)
        UpdateMusicStream(game.music);

        // Handle player input and game state updates
        game.HandleInput();
        game.Update();

        // Start drawing the frame
        BeginDrawing();

        // Clear screen with a grey background color
        ClearBackground(grey);

        // Draw the starry sky background
        for (int x = 0; x < windowWidth + background.width; x += background.width)
        {
            for (int y = 0; y < windowHeight; y += background.height)
            {   
                DrawTexture(background, x, y, WHITE);
            }
        }

        // Draw the buildings in the background
        for (int x = 0; x < windowWidth + backgroundBuildingsWidth; x += backgroundBuildingsWidth)
        {
            Rectangle source = { 0, 0, (float)backgroundBuildings.width, (float)backgroundBuildings.height };  
            Rectangle dest = { (float)x, 350.0, (float)backgroundBuildingsWidth, (float)backgroundBuildingsHeight };  
            Vector2 origin = { 0, 0 };  
            DrawTexturePro(backgroundBuildings, source, dest, origin, 0.0f, WHITE);
        }

        // Draw the dark floor background
        for (int x = 0; x < windowWidth + BackgroundFloor.width; x += BackgroundFloor.width)
        {
            for (int y = 350 + backgroundBuildingsHeight; y < windowHeight + BackgroundFloor.height * 2; y += BackgroundFloor.height)
            {   
                DrawTexture(BackgroundFloor, x, y, WHITE);
            }
        }

        // Draw UI elements: game border and score separators
        DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18, 20, yellow);
        DrawLineEx({25, 730}, {775, 730}, 3, yellow);

        // Display game status: level or game over
        if (game.run) 
        {
            std::string levelText = FormatWithLeadingZeros(game.level, 2);
            DrawTextEx(font, ("LEVEL " + levelText).c_str(), {570, 740}, 34, 2, yellow);
        } 
        else 
        {
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, yellow);
        }

        // Draw player's remaining lives as spaceship icons
        for (int i = 0; i < game.lives; i++) 
        {
            float x = 50.0 * (i + 1);
            DrawTextureV(spaceshipImage, {x, 745}, WHITE);
        }

        // Display player's current score
        DrawTextEx(font, "SCORE", {50, 25}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 50}, 34, 2, yellow);

        // Display the high score
        DrawTextEx(font, "HIGH-SCORE", {550, 25}, 34, 2, yellow);
        std::string highscoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highscoreText.c_str(), {640, 50}, 34, 2, yellow);

        // Draw game elements (spaceship, aliens, lasers, etc.)
        game.Draw();

        // End drawing the frame
        EndDrawing();
    }

    // Clean up loaded resources
    UnloadTexture(background);
    UnloadTexture(backgroundBuildings);
    UnloadTexture(BackgroundFloor);
    CloseWindow();
    CloseAudioDevice();
}
