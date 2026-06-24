#pragma once

#include "Renderer/Renderer.h"
#include "Renderer/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Action/DelayedActionManager.h"
#include "Window/Window.h"
#include "Input/Input.h"
#include "Timer/Time.h"

#include "Cannon.h"
#include "Level.h"
#include "Bubble.h"

#include <memory>
#include <iostream>

#include <SDL/SDLManager.h>
#include "Resources/ResourceLoader.h"

struct DataStates
{
    int winWidth = 360;
    int winHeight = 640;

    int logWidth = 360;
    int logHeight = 640;
};

struct CollisionResults
{
    bool hit = false;
    GridPosition hitCell = { -1, -1 };
    bool isTopCollision = false;
    bool isWallCollision = false;

};

enum GameState
{
    SHOOTING,
    COLLIDING,
    GAMEOVER
};
const float CELL_SIZE = 32.0f;
const float ROW_HEIGHT = CELL_SIZE * 0.85f;
const float FIXED_DELTA = 1.0f / 60.0f; // 60 FPS logic

class Game
{
private:

    Window m_Window;                            // Window instance
    Renderer m_Renderer;                        // Renderer instance
    TextureManager m_TextureManager;            // TextureManager instance
    Time m_Time;                                // Time instance
    Input m_Input;                              // Input instance
    AudioManager m_AudioManager;                // AudioManager instance
    DelayedActionManager m_ActionManager;       // DelayedActionManager intance

    std::unique_ptr<Cannon> m_Cannon;

    Level m_Level;                              // Level Instance
    std::unique_ptr<Bubble> m_MovingBubble;     // currently flying
    BubbleColor m_NextBubbleColor;              // what cannon will shoot next

    bool m_Running;                             // is game running
    DataStates dataStates;                      // data states with sizes
    GameState gameState;                        // current game state



public:
    // Initializes the game Window and Renderer
    bool Init();

    // Runs the main loop of the game
    void Run();

    // Closes and cleans alocated mem
    void Quit();
private:

    // Process and execute input commands
    void ProcessInput();
    // Fixed update
    void Update(float deltaTime);
    // Resource dealocation SDL
    void cleanup();
    // UI restarted game
    void RestartGame();

    // Draws
    void Render();

    // Update aiming state behave
    void UpdateAiming();
    // Update shooted bubble behave
    void UpdateShooting(float deltaTime);
    
    // GameLogic
    // Shoot a bubble
    void ShootBubble();
    // Check bubble collisions
    CollisionResults CheckCollisions();
    // Sets next bubble to shoot
    void ResetShootBubble(float x, float y, float scale);
    // Prepare next bubble color
    void PrepareNextBubbleColor();

    // UI
    void RenderGameOverUI();                        //FUTURE- MOVE TO UIMANAGER
    bool IsReplayButtonClicked(float x, float y);   //FUTURE- MOVE TO UIMANAGER

    //UTILS
    // Translates from BubbleColor to specif texture id
    std::string GetTexIdFromColor(BubbleColor color);

};

