//
// Created by julia on 14/07/2025.
//

#include "engine/logic/ActionRegistry.h"

namespace gl3::engine::combat {
    void ActionRegistry::scheduleAction(int delay, std::function<void()> action)
    {
        if (delay <= 0)
        {
            action();
        }else
        {
            actions.push_back({delay, std::move(action)});
        }

    }

    bool ActionRegistry::advance()
    {
        for (auto iter = actions.begin(); iter != actions.end();)
        {
            iter->delayInTurns--;
            if (iter->delayInTurns <= 0)
            {
                iter->action();
                iter = actions.erase(iter);
            }else
            {
                ++iter;
            }
        }
        return false;
    }
} // gl3