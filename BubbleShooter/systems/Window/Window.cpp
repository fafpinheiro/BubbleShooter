#include "Window.h"

#include <SDL3/SDL.h>

Window::Window() : m_Window(nullptr), m_Width(0), m_Height(0) {}

Window::~Window()
{
}

bool Window::Init(const std::string& title, int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_Window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);

    return m_Window != nullptr;
}

void Window::Shutdown()
{
    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

SDL_Window* Window::GetSDLWindow() const
{
    return m_Window;
}