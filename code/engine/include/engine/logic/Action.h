//
// Created by julia on 14/07/2025.
//

#pragma once
#include <functional>


namespace gl3::engine::combat {
struct Action {
    int delayInTurns;
    std::function<void()> action;
};
}
