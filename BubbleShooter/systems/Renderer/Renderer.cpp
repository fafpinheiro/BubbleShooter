#include "Renderer.h"
#include "Texture.h"
#include <iostream>

Renderer::Renderer() : m_Renderer(nullptr) {}

Renderer::~Renderer()
{
}

bool Renderer::Init(SDL_Window* window)
{
    m_Renderer = SDL_CreateRenderer(window, nullptr);
    return m_Renderer != nullptr;
}

void Renderer::Shutdown()
{
    if (m_Renderer)
    {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
    }
}

void Renderer::Clear()
{
    SDL_RenderClear(m_Renderer);
}

void Renderer::Present()
{
    SDL_RenderPresent(m_Renderer);
}

void Renderer::EnableVSync()
{
    SDL_SetRenderVSync(m_Renderer, 1);
}

void Renderer::FillRect(int x, int y, int w, int h)
{
    SDL_FRect rect;
    rect.x = static_cast<float>(x);
    rect.y = static_cast<float>(y);
    rect.w = static_cast<float>(w);
    rect.h = static_cast<float>(h);

    if (!SDL_RenderFillRect(m_Renderer, &rect))
    {
        std::cerr << "SDL_RenderFillRect failed: " << SDL_GetError() << "\n";
    }
}

void Renderer::DrawRect(int x, int y, int w, int h)
{
    SDL_FRect rect;
    rect.x = static_cast<float>(x);
    rect.y = static_cast<float>(y);
    rect.w = static_cast<float>(w);
    rect.h = static_cast<float>(h);

    if (!SDL_RenderRect(m_Renderer, &rect))
    {
        std::cerr << "SDL_RenderRect failed: " << SDL_GetError() << "\n";
    }
}

void Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
}

void Renderer::DrawTexture(Texture* texture, float x, float y, float w, float h)
{
    SDL_FRect dst { 
        .x = x, 
        .y = y,
        .w = w,
        .h = h 
    };
    SDL_FRect src{
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };
    SDL_RenderTexture(m_Renderer, texture->GetSDLTexture(), nullptr/*&src*/, &dst);
}

void Renderer::DrawTextureEx(Texture* texture, float x, float y, float w, float h, double angle)
{
    SDL_FRect dst{
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };
    SDL_FRect src{
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };
    SDL_RenderTextureRotated(m_Renderer, texture->GetSDLTexture(), nullptr/*&src*/, &dst, angle, nullptr, SDL_FLIP_NONE);
}

void Renderer::SetRenderLogicalPresentation(int logicalWidth, int logicalHeight, PresentationStyle LogicalRes)
{
    SDL_RendererLogicalPresentation sdlLogicalRes = (LogicalRes == PresentationStyle::DISABLED)
        ? SDL_LOGICAL_PRESENTATION_DISABLED : SDL_LOGICAL_PRESENTATION_LETTERBOX;
    SDL_SetRenderLogicalPresentation(m_Renderer, logicalWidth, logicalHeight, sdlLogicalRes);
}

SDL_Renderer* Renderer::GetSDLRenderer() const
{
    return m_Renderer;
}

void Renderer::WindowToLogical(float& x, float& y) const
{
    SDL_RenderCoordinatesFromWindow(m_Renderer, x, y, &x, &y);
}

