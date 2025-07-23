//
// Created by Julian Kalb on 20/06/2025.
//

#pragma once
#include "engine/Game.h"

namespace gl3::engine::ecs {
    class System {
    public:
        explicit System(Game &engine) : engine(engine) {}
        System(System &&) = delete;
        System(const System &) = delete;
        virtual ~System() = default;

    protected:
        engine::Game &engine;

        template <typename T, typename... Guids>
        bool checkIfEntityHasComponent(Guids... guids)
        {
            return (... && engine.componentManager.hasComponent<T>((guids)));
        }
    };
}