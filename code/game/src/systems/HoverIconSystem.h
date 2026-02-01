//
// Created by Julian Kalb on 01/02/2026.
//

#pragma once
#include "engine/ecs/System.h"

namespace gl3 {
    class HoverIconSystem: public engine::ecs::System {
    public:
        explicit HoverIconSystem(engine::Game& game);
        void update(engine::Game& game);
    };
}
