#include "DelayedActionManager.h"

void DelayedActionManager::Schedule(float delay, std::function<void()> action)
{
    m_Actions.push_back({ delay, action });
}

void DelayedActionManager::Update(float deltaTime)
{
    for (auto it = m_Actions.begin(); it != m_Actions.end(); )
    {
        it->timeRemaining -= deltaTime;

        if (it->timeRemaining <= 0.0f)
        {
            // execute the call
            it->action();
            it = m_Actions.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void DelayedActionManager::Clear()
{
    m_Actions.clear();
}