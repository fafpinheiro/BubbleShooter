#pragma once

class SDLManager
{
public:
    static bool Init();
    static void Shutdown();

private:
    // Inits the sdl
    static bool InitSDL();
    // Inits the sdl_mixer
    static bool InitAudio();

    // Shutdown the sdl_mixer
    static void ShutdownAudio();
    // ShutDoen sdl
    static void ShutdownSDL();

    // safety check
    static bool IsInitialized();

private:
    static bool s_Initialized; // are slds initialized
};