#pragma once

#include <SDL3/SDL.h>
#include <string>

// Forward declarations
struct SDL_Renderer;
class Texture;

enum class PresentationStyle
{ // this way i dont need to include the SDL in the TextureManager
    DISABLED,
    LETTERBOX
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Init(SDL_Window* window);
    void Shutdown();

    // Clear render target
    void Clear();               
    // Update the screen
    void Present();             
    // Syncs the Frame rate to the screen hz
    void EnableVSync();

    //  Fill rect with color
    void FillRect(int x, int y, int w, int h);
    // Draw rect in curr render target
    void DrawRect(int x, int y, int w, int h);
    // Set color for drawing operations
    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    // Draw single texture
    void DrawTexture(Texture* texture, float x, float y, float w, float h);
    // Draw single texture with angle
    void DrawTextureEx(Texture* texture, float x, float y, float w, float h, double angle);

    // Set a logical Resolution. This way we can work with the same resolution regardless the size of the window or fullscreen res of the game. SDL does the scalling
    void SetRenderLogicalPresentation(int logicalWidth, int logicalHeight, PresentationStyle LogicalRes = PresentationStyle::DISABLED);
    
    // Get sdl renderer
    SDL_Renderer* GetSDLRenderer() const;

    // Convert window coordinates to Logical Coordinates, caused by this SetRenderLogicalPresentation
    void WindowToLogical(float& x, float& y) const;

private:
    SDL_Renderer* m_Renderer;
};