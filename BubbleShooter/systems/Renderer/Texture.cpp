#include "Texture.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h> // SDL_Image for PNG/JPG loading
#include <iostream>

Texture::Texture()
{
}

Texture::~Texture()
{
    if (m_Texture)
    {
        SDL_DestroyTexture(m_Texture);
        m_Texture = nullptr;
    }
}

bool Texture::LoadFromFile(SDL_Renderer* renderer, const std::string& path, TextureScaleMode scaleMode)
{
    if (!renderer)
    {
        std::cerr << "Renderer is null in Texture::LoadFromFile\n";
        return false;
    }

    SDL_Surface* surface = IMG_Load(path.c_str()); //loads the image into CPU memory
    if (!surface)
    {
        std::cerr << "Failed to load image: " << path << " SDL_image Error: " << "\n";
        return false;
    }

    // EDIT image here
    //

    //uploads the surface into GPU memory as a texture.
    m_Texture = SDL_CreateTextureFromSurface(renderer, surface); 
    SDL_DestroySurface(surface);

    if (!m_Texture)
    {
        std::cerr << "Failed to create texture from surface: " << path << " SDL Error: " << SDL_GetError() << "\n";
        return false;
    }


    SDL_ScaleMode sdlMode = (scaleMode == TextureScaleMode::Linear) ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST;

    // Set pixel scale mode
    SDL_SetTextureScaleMode(m_Texture, sdlMode);

    return true;
}
