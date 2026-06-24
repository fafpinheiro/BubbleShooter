#pragma once

#include <string>
#include <SDL3_mixer/SDL_mixer.h>
class Audio
{
public:
    Audio() = default;
    ~Audio();

    // Load audio from file with path
    bool LoadFromFile(MIX_Mixer* renderer, const std::string& path, bool predecode);
    // Get the Mix Audio
    MIX_Audio* GetMixAudio() const { return m_Audio; }

private:
    MIX_Audio* m_Audio = nullptr;           // Audio Mix
};