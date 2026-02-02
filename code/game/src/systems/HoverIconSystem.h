//
// Created by Julian Kalb on 01/02/2026.
//

#pragma once
#include "engine/ecs/ecs.h"
#include "engine/ecs/System.h"

struct Visibility;
struct UvOffset;
struct Unit;

namespace gl3 {
    class HoverIconSystem: public engine::ecs::System {
    public:
        explicit HoverIconSystem(engine::Game& game);
        void update(engine::Game& game);
    private:
        void HandleIconLogic(engine::Game& game);
        bool HelperVisibilityBaseCases(Unit& unit, Visibility* pVisibility_C) const;
    };
}
