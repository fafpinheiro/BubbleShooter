#pragma once

#include <vector>
#include <functional>

class DelayedActionManager
{
public:
    // Shedules a new call
    void Schedule(float delay, std::function<void()> action);
    // Runs the sheduled calls
    void Update(float deltaTime);
    // Cleans all actions
    void Clear();

private:
    struct DelayedAction
    {
        float timeRemaining;
        std::function<void()> action;
    };

    std::vector<DelayedAction> m_Actions;       // List of delayed actions
};