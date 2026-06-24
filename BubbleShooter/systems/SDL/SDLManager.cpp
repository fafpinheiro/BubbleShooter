#include "SDLManager.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <iostream>

bool SDLManager::s_Initialized = false;

bool SDLManager::Init()
{
    if (s_Initialized)
        return true;

    if (!InitSDL()) return false;
    if (!InitAudio()) return false;

    s_Initialized = true;
    return true;
}

void SDLManager::Shutdown()
{
    if (!s_Initialized)
        return;

    ShutdownAudio();
    ShutdownSDL();

    s_Initialized = false;
}

bool SDLManager::InitSDL()
{
    // Initialize SDL core systems
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool SDLManager::InitAudio()
{
    // Initialize SDL_mixer
    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed: " << SDL_GetError() << std::endl;

        return false;
    }
    return true;
}

void SDLManager::ShutdownAudio()
{
    MIX_Quit();
}

void SDLManager::ShutdownSDL()
{
    SDL_Quit();
}

bool SDLManager::IsInitialized()
{
    return s_Initialized;
}
