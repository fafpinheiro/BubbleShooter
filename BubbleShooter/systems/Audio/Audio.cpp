#include "Audio.h"
#include <iostream>


Audio::~Audio()
{
    if (m_Audio)
    {
        MIX_DestroyAudio(m_Audio);
        m_Audio = nullptr;
    }
}

bool Audio::LoadFromFile(MIX_Mixer* mixer, const std::string& path, bool predecode)
{
    if (!mixer) return false;

    m_Audio = MIX_LoadAudio(mixer, path.c_str(), predecode);
    if (!m_Audio)
    {
        SDL_Log("Couldn't load audio from %s: %s", path.c_str(), SDL_GetError());
        return false;
    }

    return true;
}