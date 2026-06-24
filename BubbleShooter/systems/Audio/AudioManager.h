#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <SDL3_mixer/SDL_mixer.h>
#include "Audio.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool Init();   // initialize SDL3_mixer
    void Shutdown();


    // Load a SFX
    bool LoadSound(const std::string& id, const std::string& path, bool predecode = true);
    // Play a SFX
    void PlaySound(const std::string& id);

    // Load a music
    bool LoadMusic(const std::string& id, const std::string& path, bool predecode = true);
    // Play a music
    void PlayMusic(const std::string& id, int loops = -1);

    // Stops the musics
    void StopMusic();
    // Pauses the musics
    void PauseMusic();
    // Replays the musics
    void ResumeMusic();

    // Clear loaded SFXs
    void ClearSounds();
    // Clear loaded musics
    void ClearMusics();

private:
    MIX_Mixer* m_Mixer = nullptr;                                           // SDL mixer
    std::unordered_map<std::string, std::unique_ptr<Audio>> m_Sounds;       // Sounds loaded
    std::unordered_map<std::string, std::unique_ptr<Audio>> m_Musics;       // Musics loaded

    // Only MUSIC is persistent
    MIX_Track* m_CurrentMusicTrack = nullptr;                               // Music track for persistent musics
};