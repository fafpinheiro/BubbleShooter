// Time.cpp
#include "Time.h"

Time::Time()
{
    Reset();
}
Time::~Time()
{
}

float Time::GetDeltaTime()
{
    Uint64 now = SDL_GetTicks();
    float delta = (now - m_Last) / 1000.0f;
    m_Last = now;
    return delta;
}

void Time::Reset()
{
    m_Last = SDL_GetTicks();
}