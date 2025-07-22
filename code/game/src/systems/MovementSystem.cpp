//
// Created by julia on 22/07/2025.
//

#include "MovementSystem.h"

#include "../components/UnitState.h"
#include "engine/sceneGraph/Transform.h"
#include "../components/unitTypes/Unit.h"

using gl3::engine::sceneGraph::Transform;
namespace gl3 {

    MovementSystem::MovementSystem(engine::Game& game):
    System(game)
    {
    }

    void MovementSystem::moveTo(engine::Game& game, glm::vec3 goalPositon, float deltaTime)
    {
        game.componentManager.forEachComponent<Transform>([&](Transform &transform)
        {
            if (game.componentManager.hasComponent<Unit>(transform.entity()))
            {
                auto unitCategory = game.componentManager.getComponent<Unit>(transform.entity()).category;
                if (unitCategory == UnitCategory::INFANTRY)
                {
                    move(transform, goalPositon, deltaTime);
                }
            }
        });
    }

    void MovementSystem::move(Transform& root, glm::vec3 goalPosition, float deltatime)
    {
        glm::vec3 direction = goalPosition - root.localPosition;
        float distanceToGoal = glm::length(direction);
        auto children = root.getChildTransforms();
        for (auto& childTransform : children)
        {
            auto& unitState_C = engine.componentManager.getComponent<UnitState>(childTransform->entity());
            if (unitState_C.state != State::MOVING) continue;
            auto speed = unitState_C.movementSpeed;

            if (distanceToGoal - unitState_C.traveledDistance >= 0)
            {
                childTransform->localPosition += speed * deltatime * direction;
                unitState_C.traveledDistance += 2* deltatime * speed;
            }
        }
    }
} // gl3