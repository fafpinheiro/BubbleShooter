#pragma once
#include <SDL3/SDL.h>
// Game time
class Time
{
public:
    Time();
    ~Time();
    // Gets delta time
    float GetDeltaTime(); // in seconds
    // Resets clock
    void Reset();


private:
    Uint64 m_Last; // store ticks
};