#include "AudioManager.h"
#include <SDL3/SDL_log.h>

AudioManager::AudioManager() = default;

AudioManager::~AudioManager()
{
}

bool AudioManager::Init()
{
    m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!m_Mixer)
    {
        SDL_Log("Couldn't create mixer: %s", SDL_GetError());
        return false;
    }
    return true;
}

void AudioManager::Shutdown()
{
    StopMusic();
    ClearSounds();
    ClearMusics();

    if (m_Mixer)
    {
        MIX_DestroyMixer(m_Mixer);
        m_Mixer = nullptr;

    }
}

bool AudioManager::LoadSound(const std::string& id, const std::string& path, bool predecode)
{
    auto tmpSound = m_Sounds.find(id);
    if (tmpSound != m_Sounds.end())
    {
        SDL_Log("Sound already loaded");

        return false;
    }

    if (!m_Mixer) return false;
    
    auto audio = std::make_unique<Audio>();
    if (!audio->LoadFromFile(m_Mixer,path, predecode)) 
    {
        SDL_Log("Couldn't load audio from %s: %s", path.c_str(), SDL_GetError());
        return false;
    }
    
    m_Sounds[id] = std::move(audio);
    return true;
}

void AudioManager::PlaySound(const std::string& id)
{
    auto it = m_Sounds.find(id);
    if (it != m_Sounds.end())
    {
        // Calls SFX
        MIX_PlayAudio(m_Mixer, it->second.get()->GetMixAudio());
    }
}

bool AudioManager::LoadMusic(const std::string& id, const std::string& path, bool predecode)
{
    auto tmpMusic = m_Musics.find(id);
    if (tmpMusic != m_Musics.end())
    {
        SDL_Log("Music already loaded");

        return false;
    }

    if (!m_Mixer) return false;

    // Creates new audio
    auto music = std::make_unique<Audio>();
    if (!music->LoadFromFile(m_Mixer, path, predecode))
    {
        SDL_Log("Couldn't load music from %s: %s", path.c_str(), SDL_GetError());
        return false;
    }

    m_Musics[id] = std::move(music);
    return true;
}

void AudioManager::PlayMusic(const std::string& id, int loops)
{
    auto it = m_Musics.find(id);
    if (it != m_Musics.end())
    {
        // destroy old music track
        if (m_CurrentMusicTrack)
        {
            MIX_StopTrack(m_CurrentMusicTrack, 0);
            MIX_DestroyTrack(m_CurrentMusicTrack);
            m_CurrentMusicTrack = nullptr;
        }
        // Create new track
        m_CurrentMusicTrack = MIX_CreateTrack(m_Mixer);
        // links music to track
        MIX_SetTrackAudio(m_CurrentMusicTrack, it->second.get()->GetMixAudio());

        // Sets music play
        MIX_PlayTrack(m_CurrentMusicTrack, 0);
        MIX_SetTrackGain(m_CurrentMusicTrack, 0.05f);
        // Sets music loop times
        MIX_SetTrackLoops(m_CurrentMusicTrack, loops);
    }
}

void AudioManager::StopMusic()
{
    if (m_CurrentMusicTrack)
    {
        MIX_StopTrack(m_CurrentMusicTrack,0);
        MIX_DestroyTrack(m_CurrentMusicTrack);
        m_CurrentMusicTrack = nullptr;
    }
}

void AudioManager::PauseMusic()
{
    if (m_CurrentMusicTrack)
        MIX_PauseTrack(m_CurrentMusicTrack);
}

void AudioManager::ResumeMusic()
{
    if (m_CurrentMusicTrack)
        MIX_ResumeTrack(m_CurrentMusicTrack);
}

void AudioManager::ClearSounds()
{
    if (m_Mixer)
    {
        m_Sounds.clear();
    }
}
void AudioManager::ClearMusics()
{
    if (m_Mixer)
    {
        m_Musics.clear();
    }
}
