//
// Created by julia on 14/07/2025.
//

#pragma once
#include "Action.h"

namespace gl3::engine::combat {
class ActionRegistry {
public:
    void scheduleAction(int delay, std::function<void()> action);
    void advance();
private:
    std::list<Action> actions;
};
}
