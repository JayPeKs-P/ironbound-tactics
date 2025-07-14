//
// Created by julia on 14/07/2025.
//

#pragma once
#include "Action.h"

namespace gl3::engine::combat {
class ActionRegistry {
public:
    static void scheduleAction(int delay, std::function<void()> action, std::list<Action>* actions);
    static void advance(std::list<Action>* actions);
};
}
