#pragma once
#include <string>

//Forward declations
struct SDL_Window;


class Window
{
public:
    Window();
    ~Window();

    bool Init(const std::string& title, int width, int height);
    // Destroy SDL window
    void Shutdown();

    // Gets sdl win
    SDL_Window* GetSDLWindow() const;

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    SDL_Window* m_Window;   // SDL Window
    int m_Width;            // Window width
    int m_Height;           // Window height
};