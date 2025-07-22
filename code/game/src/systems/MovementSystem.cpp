//
// Created by julia on 22/07/2025.
//

#include "MovementSystem.h"

#include <iostream>

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
                    glm::vec3 direction = goalPositon - transform.localPosition;
                    bool finished = false;
                    // finished = moveStraight(transform, direction, deltaTime);
                    finished = moveCurved(transform, glm::vec3(1.75f, 0.0f ,0.0f), 1.0f, deltaTime);
                    if (finished)
                    {
                        std::cout << "finished"<<std::endl;
                        finished = false;
                    }
                }
            }
        });
    }

    bool MovementSystem::moveStraight(Transform& root, glm::vec3 direction, float deltatime)
    {
        bool reached = false;
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
            }else
            {
                unitState_C.state = State::IDLE;
                reached = true;
            }
        }
        return reached;
    }

    bool MovementSystem::moveCurved(Transform& root, glm::vec3 goal, float compression, float deltatime)
    {
        bool reached = false;

        glm::vec3 start = root.localPosition;
        glm::vec3 forward = glm::normalize(goal - start);
        glm::vec3 worldUp = glm::vec3(0, 1, 0);

        glm::vec3 side = glm::normalize(glm::cross(forward, worldUp));
        glm::vec3 up = glm::normalize(glm::cross(side, forward));

        glm::vec3 mid = (start + goal) * 0.5f + compression * up;
        float totalDistance = glm::length(goal - start);

        glm::vec3 direction = goal - root.localPosition;
        float distanceToGoal = glm::length(direction);
        auto children = root.getChildTransforms();
        for (auto& childTransform : children)
        {
            auto& unitState_C = engine.componentManager.getComponent<UnitState>(childTransform->entity());
            auto speed = 3.0f;

            float t = unitState_C.traveledDistance / totalDistance;
            if (t > 1.0f) t = 1.0f;


            if (distanceToGoal - unitState_C.traveledDistance >= 0)
            {
                float t = unitState_C.traveledDistance / totalDistance;
                if (t > 1.0f) t = 1.0f;

                glm::vec3 direction =
                    2.0f * (1.0f - t) * (mid - start) +
                    2.0f * t * (goal - mid);
                direction = glm::normalize(direction);

                childTransform->localPosition += speed * deltatime * direction;
                unitState_C.traveledDistance += speed * deltatime;
            }else
            {
                unitState_C.state = State::IDLE;
                reached = true;
            }
        }
        return reached;
    }
} // gl3