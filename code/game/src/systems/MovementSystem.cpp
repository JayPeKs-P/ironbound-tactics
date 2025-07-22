//
// Created by julia on 22/07/2025.
//

#include "MovementSystem.h"
#include "engine/sceneGraph/Transform.h"
#include "../components/unitTypes/Unit.h"

using gl3::engine::sceneGraph::Transform;
namespace gl3 {
    void MovementSystem::moveTo(engine::Game& game, glm::vec3 goalPositon)
    {
        game.componentManager.forEachComponent<Transform>([&](Transform &transform)
        {
            int createdInstances = 0;
            int maxInstances = 0;
            if (game.componentManager.hasComponent<Unit>(transform.entity()))
            {
                auto unitCategory = game.componentManager.getComponent<Unit>(transform.entity()).category;

            }
        });
    }
} // gl3