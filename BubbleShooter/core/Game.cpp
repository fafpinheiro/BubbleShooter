#include "Game.h"
#include <filesystem>
#include <string>

bool Game::Init()
{
    if (!SDLManager::Init())
    {
        std::cout << "Error initializing SDL3 " << std::endl;
        return false;
    }

    // create the window
    if (!m_Window.Init("Bubble Shooter", dataStates.winWidth, dataStates.winHeight))
    {
        std::cout << "Error Creating Window " << std::endl;

        m_Window.Shutdown();
        return false;
    }

    // create the renderer
    if (!m_Renderer.Init(m_Window.GetSDLWindow()))
    {
        std::cout << "Error creating the Renderer" << std::endl;

        cleanup();
        return false;
    }
    if (!m_AudioManager.Init())
    {
        std::cout << "Error creating the AudioManager" << std::endl;
        m_AudioManager.Shutdown();
        return false;
  
    }
    //m_Renderer.EnableVSync();

    // Set logical resolution (once)
    m_Renderer.SetRenderLogicalPresentation(dataStates.logWidth, dataStates.logHeight, PresentationStyle::LETTERBOX);

    //LoadResources();
    ResourceLoader::LoadAll(m_Renderer, m_TextureManager, m_AudioManager);

    // Init cannon
    m_Cannon = std::make_unique<Cannon>(static_cast<float>(dataStates.logWidth/2)- CELL_SIZE, dataStates.logHeight- CELL_SIZE, 300.0f);

    // Prepare bubbles to shoot
    PrepareNextBubbleColor();
    m_MovingBubble = std::make_unique<Bubble>(m_Cannon->GetX(), m_Cannon->GetY() - CELL_SIZE, CELL_SIZE, m_NextBubbleColor);
    PrepareNextBubbleColor();

    // Init playing grid
    m_Level.InitializeGrid(dataStates.logWidth, dataStates.logHeight, CELL_SIZE, ROW_HEIGHT);

    // Sets SHOOTING state
    gameState = SHOOTING;

    // Inits background music
    m_AudioManager.PlayMusic("elevatorMusic", -1);

    return true;
}

void Game::Run()
{
    float accumulator = 0.0f;

    m_Running = true;

    while (m_Running && !m_Input.QuitRequested())
    {
        float dt = m_Time.GetDeltaTime();
        accumulator += dt;

        // Process player inputs
        ProcessInput();
        
        // Fixed update loop
        while (accumulator >= FIXED_DELTA)
        {
            float mouseX = m_Input.GetMouseX();
            float mouseY = m_Input.GetMouseY();

            // Conver mouse position from window to logic space
            m_Renderer.WindowToLogical(mouseX, mouseY);

            // Updates controllable cannon state
            m_Cannon->Update(mouseX - CELL_SIZE /2, mouseY, FIXED_DELTA);

            // Computes if any delayed actor to call
            m_ActionManager.Update(FIXED_DELTA);

            // Game main logic
            Update(FIXED_DELTA);


            accumulator -= FIXED_DELTA;
        }
        // Draw
        Render();
    }
}

void Game::Quit()
{
    cleanup();
}

void Game::ProcessInput()
{
    m_Input.Update();

    // Test If any mouse clicked
    if (m_Input.IsMouseButtonPressed(MouseButton::Left))
    {
        if (gameState == GameState::SHOOTING)
        {
            ShootBubble();
            gameState = COLLIDING;
            return;
        }
        if (gameState == GameState::GAMEOVER)
        {

            float mouseX = m_Input.GetMouseX();
            float mouseY = m_Input.GetMouseY();

            m_Renderer.WindowToLogical(mouseX, mouseY);

            // Checks if restar button was pressed
            if (IsReplayButtonClicked(mouseX, mouseY))
            {
                RestartGame();
            }
            return;
        }
    }
}

void Game::Update(float deltaTime)
{
    switch (gameState) {
    case SHOOTING:
        UpdateAiming();
        break;
    case COLLIDING:
        UpdateShooting(deltaTime);
        break;
    case GAMEOVER:
        // Managed in ProcessInput
        m_AudioManager.StopMusic();

        break;
    }
}

void Game::Render()
{
    if (gameState == GameState::GAMEOVER)
    {
        m_Renderer.Clear();

        RenderGameOverUI();
        m_Renderer.Present();

    }
    else {
        m_Renderer.Clear();

        // perform drawing commands
        //m_Renderer.SetDrawColor(26, 84, 63, 255);
        m_Renderer.SetDrawColor(0, 77, 57, 255);
        // resterize color
        m_Renderer.Clear();

        // PLayer
        m_Renderer.DrawTextureEx(m_TextureManager.GetTexture("cannon"), m_Cannon->GetX(), m_Cannon->GetY(), CELL_SIZE, CELL_SIZE, m_Cannon->GetAngle());//
        m_Renderer.DrawTextureEx(m_TextureManager.GetTexture(GetTexIdFromColor(m_NextBubbleColor)), m_Cannon->GetX() + 32.0f, m_Cannon->GetY(), CELL_SIZE, CELL_SIZE, m_Cannon->GetAngle());//

        // Draw bubble in cannon
        Texture* tex = m_TextureManager.GetTexture(GetTexIdFromColor(m_MovingBubble->GetColor()));
        // Draw shooting bubble
        m_Renderer.DrawTexture(tex, m_MovingBubble->GetX(), m_MovingBubble->GetY(), CELL_SIZE, CELL_SIZE); // near cannon

        // Draw Level
        m_Level.Render(m_Renderer, m_TextureManager);

        // swap buffers and present
        m_Renderer.Present();
    }
}

void Game::RestartGame()
{
    // Clean old playing grid
    m_Level.CleanGrid();

    // Init cannon
    m_Cannon = std::make_unique<Cannon>(static_cast<float>(dataStates.logWidth / 2) - CELL_SIZE, dataStates.logHeight - CELL_SIZE, 300.0f);

    // Init Shooting bubble
    PrepareNextBubbleColor();
    m_MovingBubble = std::make_unique<Bubble>(m_Cannon->GetX(), m_Cannon->GetY() - CELL_SIZE, CELL_SIZE, m_NextBubbleColor);
    PrepareNextBubbleColor();

    // Init new playing Grid
    m_Level.InitializeGrid(dataStates.logWidth, dataStates.logHeight, CELL_SIZE, ROW_HEIGHT);

    // Set to SHOOTING state
    gameState = SHOOTING;

    // Play background music
    m_AudioManager.PlayMusic("elevatorMusic", -1);
}

void Game::UpdateAiming()
{
    // Convert from polar to cartesian coords
    float angle = m_Cannon->GetAngle() * (3.14159265f / 180.0f);
    float x = m_Cannon->GetX() + CELL_SIZE * cos(-angle);
    float y = m_Cannon->GetY() + CELL_SIZE * sin(angle);
    // Set the bubble in the cannon barrel
    m_MovingBubble->SetPosition(x, y);
}

void Game::UpdateShooting(float deltaTime)
{
    // Sim bubble movement
    m_MovingBubble->Update(deltaTime);

    CollisionResults result = CheckCollisions();
    if (result.hit)
    {
        if (result.isWallCollision)
        {
            m_MovingBubble->BounceX(1);
            m_AudioManager.PlaySound("wallSX");
        }
        else
        {
            GridPosition finalCell;

            if (result.isTopCollision)
            {
                // Direct placement on top row
                //finalCell = result.hitCell;
                finalCell = m_Level.FindAttachCell(m_MovingBubble.get(), result.hitCell.row, result.hitCell.col);

            }
            else
            {
                // Attach logic
                finalCell = m_Level.FindAttachCell(m_MovingBubble.get(), result.hitCell.row, result.hitCell.col);
            }

            if(finalCell.row >= m_Level.GetNRows() - 3)
            {
                gameState = GAMEOVER;
                return;
            }
            // Move ownership
            m_Level.SetGridPos(std::move(m_MovingBubble), finalCell.row, finalCell.col);

            // MATCH logic
            std::vector<GridPosition> group = m_Level.GetConnectedSameColor(finalCell.row, finalCell.col);

            // if any sequece of 3 bubbles of same color was created
            if (group.size() >= 3)
            {
                m_Level.Remove(group);

                m_AudioManager.PlaySound("multiPopSX");
                m_ActionManager.Schedule(0.3f, [this]()
                    {
                        if (m_Level.RemoveFloating())
                        {
                            m_AudioManager.PlaySound("floatingSX");
                            // Test if all grid was cleaned
                            if (m_Level.IsGridEmpty()) {
                                gameState = GAMEOVER;
                                return;
                            }
                        }
                    });
                // Test if all grid was cleaned
                if (m_Level.IsGridEmpty()) {
                    gameState = GAMEOVER;
                    return;
                }
            }
            else
            {
                m_AudioManager.PlaySound("attachingSX");
            }
            // Spawn next bubble
            ResetShootBubble(m_Cannon->GetX(), m_Cannon->GetY() - CELL_SIZE, CELL_SIZE);
            gameState = SHOOTING;
        }
    }

}

void Game::ShootBubble()
{
    if (!m_MovingBubble || m_MovingBubble->GetState() != BubbleState::Ready) return;

    float speed = 800.0f;
    float angle = m_Cannon->GetAngle() * (3.14159265f / 180.0f);
    float vx = speed * cos(-angle);
    float vy = -speed * sin(-angle);
    
    m_MovingBubble->SetVelocity(vx, vy);
    m_AudioManager.PlaySound("shootingSX");
}

CollisionResults Game::CheckCollisions()
{
    CollisionResults result;
    // Check Wall
    if (m_MovingBubble->GetX() <= 0 || m_MovingBubble->GetX() + m_MovingBubble->GetScale() >= dataStates.logWidth)
    {
        result.hit = true;
        GridPosition collisionCoords = m_Level.WorldToGrid(
            m_MovingBubble->GetX(),
            m_MovingBubble->GetY()
        );
        result.hitCell = { collisionCoords.row, collisionCoords.col };
        result.isWallCollision = true;


    }else

    // Check top 
    if (m_MovingBubble->GetY() <= 0)
    {
        GridPosition collisionCoords = m_Level.WorldToGrid(
            m_MovingBubble->GetX(),
            m_MovingBubble->GetY()
        );

        result.hit = true;
        result.hitCell = { collisionCoords.row, collisionCoords.col };
        result.isTopCollision = true;
    }
    else
    {
        // Check grid collision
        GridPosition collisionCoords = m_Level.CheckBubbleCollision(
            m_MovingBubble->GetX(),
            m_MovingBubble->GetY(),
            m_MovingBubble->GetScale()
        );

        if (collisionCoords.row != -1)
        {
            result.hit = true;
            result.hitCell = { collisionCoords.row, collisionCoords.col };
        }
    }
    return result;
}

void Game::ResetShootBubble(float x, float y, float scale)
{
    // Reset shooting bubble
    m_MovingBubble.reset();

    m_MovingBubble = std::make_unique<Bubble>(x, y, scale, m_NextBubbleColor);
    // new bubble in cannon
    PrepareNextBubbleColor();
}

void Game::PrepareNextBubbleColor()
{
    int r = rand() % 4;
    m_NextBubbleColor = static_cast<BubbleColor>(r);
}

bool Game::IsReplayButtonClicked(float x, float y)
{
    const int btnW = 220;
    const int btnH = 70;

    int btnX = dataStates.logWidth / 2 - btnW / 2;
    int btnY = dataStates.logHeight / 2 + 40;

    return (x >= btnX && x <= btnX + btnW &&
        y >= btnY && y <= btnY + btnH);
}

std::string Game::GetTexIdFromColor(BubbleColor color)
{
    std::string tmp;
    switch (color)
    {
        case BubbleColor::Red:
            tmp = "bubble_circle";
            break;
        case BubbleColor::Green:
            tmp = "bubble_x";
            break;
        case BubbleColor::Blue:
            tmp = "bubble_square";
            break;
        case BubbleColor::Yellow:
            tmp = "bubble_triangle";
            break;
    }
    return tmp;
}

void Game::RenderGameOverUI()
{
    int screenW = dataStates.logWidth;
    int screenH = dataStates.logHeight;

    // Dark full screen overlay
    m_Renderer.SetDrawColor(0, 0, 0, 180);
    m_Renderer.FillRect(0, 0, screenW, screenH);

    // Panel
    int panelW = 400;
    int panelH = 250;
    int panelX = screenW / 2 - panelW / 2;
    int panelY = screenH / 2 - panelH / 2;

    m_Renderer.SetDrawColor(0, 192, 255, 255);
    m_Renderer.FillRect(panelX, panelY, panelW, panelH);

    m_Renderer.SetDrawColor(255, 255, 255, 255);
    m_Renderer.DrawRect(panelX, panelY, panelW, panelH);


    // Button
    float btnW = 220;
    float btnH = 70;
    float btnX = screenW / 2.0f - btnW / 2.0f;
    float btnY = screenH / 2.0f + 40;

    m_Renderer.DrawTexture(m_TextureManager.GetTexture("restartButton"), btnX, btnY, btnW, btnH);
}

void Game::cleanup()
{
    m_ActionManager.Clear();

    m_TextureManager.Clear();

    m_AudioManager.Shutdown();

    m_Renderer.Shutdown();

    m_Window.Shutdown();

    SDLManager::Shutdown();
}
