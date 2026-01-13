///
// Created by julia on 22/07/2025.
//

#include "MovementSystem.h"
#include "engine/Game.h"

#include "CombatController.h"
#include "../components/unitTypes/UnitCategory.h"
#include "engine/util/Debug.h"


#include "../components/TagComponent.h"
#include "../components/UnitState.h"
#include "engine/sceneGraph/Transform.h"
#include "../components/unitTypes/Unit.h"

using gl3::engine::sceneGraph::Transform;
namespace gl3 {
    MovementSystem::event_t MovementSystem::finishAnimation;
    MovementSystem::event_t MovementSystem::finishedAllAnimations;

    MovementSystem::MovementSystem(engine::Game& game):
    System(game)
    {
        CombatController::onBeforeDamageStep.addListener([&]()
        {
            moveTo(engine, engine.getDeltaTime());
        });
#ifdef DEBUG_MODE
        finishAnimation.addListener([&](float delay)
        {
            countdown = delay;
            DEBUG_LOG(
                << "TRIGGERED EVENT: 'finishedAnimation'"
                );
        });
#endif

        CombatController::onBeforeAttack.addListener([&](guid_t unit, guid_t unused, int amount)
        {
            if (!engine.entityManager.checkIfEntityHasComponent<Unit>(unit)) throw("MovementSystem::MovementSystem() onBeforeAttack.addListener missing unit_C");
            auto unit_C = &engine.componentManager.getComponent<Unit>(unit);

            if (unit_C->category == UnitCategory::INFANTRY){
                auto& root = engine.componentManager.getComponent<Transform>(unit);
                auto& tag = engine.componentManager.getComponent<TagComponent>(unit).value;
                if (tag == Tag::PLAYER)
                {
                    setMoving(root, playerPendingPosition, amount, State::IDLE);
                }else if (tag == Tag::ENEMY)
                {
                    setMoving(root, enemyPendingPosition, amount, State::IDLE);
                }
            }
        });
        CombatController::onAttack.addListener([&](guid_t unit, guid_t target, int amount)
        {
            if (!engine.entityManager.checkIfEntityHasComponent<Unit>(unit, target)) throw("MovementSystem::MovementSystem() onAttack.addListener missing unit_C");
            auto unit_C = &engine.componentManager.getComponent<Unit>(unit);
            switch (unit_C->category)
            {
            case UnitCategory::INFANTRY:
                auto& actorRoot = engine.componentManager.getComponent<Transform>(unit);
                auto& targetRoot = engine.componentManager.getComponent<Transform>(target);
                    setMoved(actorRoot, targetRoot, amount, State::PREPARING);
                break;
            }
        });
        CombatController::onAfterAttack.addListener([&](guid_t unit, guid_t unused, int amount)
        {

        });
    }

    void MovementSystem::moveTo(engine::Game& game, float deltaTime)
    {
        m_bAllAnimationsFinished = true;
        game.componentManager.forEachComponent<Transform>([&](Transform &transform)
        {
            if (game.componentManager.hasComponent<UnitState>(transform.entity()))
            {
                auto root = transform.getParent();
                auto unitState_C = game.componentManager.getComponent<UnitState>(transform.entity());

                switch (unitState_C.state)
                {
                case State::MOVING:
                    glm::vec3 directionPrep = unitState_C.goal - root->localPosition;
                    moveStraight(transform, directionPrep, deltaTime, State::PREPARING, 3);
                    break;
                case State::PREPARING:
                    break;
                case State::MOVED:
                    glm::vec3 directionMov;
                    if (unitState_C.m_iTarget > engine::ecs::invalidID)
                    {
                        auto pTargetTransform = &engine.componentManager.getComponent<Transform>(unitState_C.m_iTarget);
                        directionMov = pTargetTransform->localPosition - transform.localPosition;
                        unitState_C.goal = pTargetTransform->localPosition;
                    }
                    moveStraight(transform, directionMov, deltaTime, State::RESETTING, 15);
                    break;
                case State::RESETTING:
                    glm::vec3 directionRes = glm::vec3(unitState_C.oldPos.x - root->localPosition.x, 0 ,0) * 0.8f;
                    moveStraight(transform, directionRes, deltaTime, State::IDLE, 3);
                    break;
                case State::IDLE:
                    break;
                }
            }
        });
        if (m_bAllAnimationsFinished)
        {
            countdown -= deltaTime;
        }
        else countdown = 0.5f;

        if (countdown <= 0 && m_bAllAnimationsFinished)
        {
            finishedAllAnimations.invoke(true);
            countdown = 0.5f;
        }
    }

    void MovementSystem::moveStraight(Transform& transform, glm::vec3 direction, float deltatime, State endState, float delay)
    {
        float distanceToGoal = glm::length(direction);
        auto& unitState_C = engine.componentManager.getComponent<UnitState>(transform.entity());
        auto speed = unitState_C.movementSpeed;

        if (distanceToGoal - unitState_C.traveledDistance >= 0)
        {
            transform.localPosition += speed * deltatime * direction;
            unitState_C.traveledDistance += 2* deltatime * speed;
            m_bAllAnimationsFinished = false;
        }else
        {
            unitState_C.oldPos = transform.localPosition;
            unitState_C.state = endState;
            finishAnimation.invoke(delay);
            if (unitState_C.state == State::IDLE)
            {
                transform.localPosition = transform.getParent()->localPosition + unitState_C.relativeVec;
            }
        }
    }

    // void MovementSystem::moveCurved(Transform& root, glm::vec3 goal, float compression, float deltatime)
    // {
    //     glm::vec3 start = root.localPosition;
    //     glm::vec3 forward = glm::normalize(goal - start);
    //     glm::vec3 worldUp = glm::vec3(0, 1, 0);
    //
    //     glm::vec3 side = glm::normalize(glm::cross(forward, worldUp));
    //     glm::vec3 up = glm::normalize(glm::cross(side, forward));
    //
    //     glm::vec3 mid = (start + goal) * 0.5f + compression * up;
    //     float totalDistance = glm::length(goal - start);
    //
    //     glm::vec3 direction = goal - root.localPosition;
    //     float distanceToGoal = glm::length(direction);
    //     auto children = root.getChildTransforms();
    //     for (auto& childTransform : children)
    //     {
    //         auto& unitState_C = engine.componentManager.getComponent<UnitState>(childTransform->entity());
    //         if (unitState_C.state != State::MOVING) continue;
    //         auto speed = 3.0f;
    //
    //         float t = unitState_C.traveledDistance / totalDistance;
    //         if (t > 1.0f) t = 1.0f;
    //
    //
    //         if (distanceToGoal - unitState_C.traveledDistance >= 0)
    //         {
    //             float t = unitState_C.traveledDistance / totalDistance;
    //             if (t > 1.0f) t = 1.0f;
    //
    //             glm::vec3 direction =
    //                 2.0f * (1.0f - t) * (mid - start) +
    //                 2.0f * t * (goal - mid);
    //             direction = glm::normalize(direction);
    //
    //             childTransform->localPosition += speed * deltatime * direction;
    //             unitState_C.traveledDistance += speed * deltatime;
    //         }else
    //         {
    //             unitState_C.state = State::IDLE;
    //             finishAnimation.invoke(true);
    //         }
    //     }
    // }

    void MovementSystem::setMoving(Transform& root, glm::vec3 goalPosition, int amount, State initialState)
    {
        int counter = 0;
        for (auto& childTransform : root.getChildTransforms())
        {
            auto& unitState_C = engine.componentManager.getComponent<UnitState>(childTransform->entity());
            if (unitState_C.state == initialState)
            {
                unitState_C.relativeVec = childTransform->localPosition - root.localPosition;
                unitState_C.traveledDistance = 0;
                unitState_C.goal = goalPosition;
                unitState_C.state = State::MOVING;

                counter++;
                if (counter >= amount)
                {
                    break;
                }
            }
        }
    }

    void MovementSystem::setMoved(Transform& root, Transform& goalPosition, int amount, State initialState)
    {
        int counter = 0;
        for (auto& childTransform : root.getChildTransforms())
        {
            auto unitState_C = &engine.componentManager.getComponent<UnitState>(childTransform->entity());
            if (unitState_C->state == initialState)
            {
                size_t currentSmalestSize = 5000;
                guid_t iTargetEntity = engine::ecs::invalidID;
                for (auto childTransformTarget : goalPosition.getChildTransforms())
                {
                    auto unitStateTarget_C = &engine.componentManager.getComponent<UnitState>(childTransformTarget->entity());
                    if (unitStateTarget_C->m_TargetedBy.empty())
                    {
                        iTargetEntity = childTransformTarget->entity();
                        break;
                    }
                    if (currentSmalestSize > unitStateTarget_C->m_TargetedBy.size())
                    {
                        currentSmalestSize = unitStateTarget_C->m_TargetedBy.size();
                        iTargetEntity = childTransformTarget->entity();
                    }
                }
                auto pTarget = engine.componentManager.getComponent<UnitState>(iTargetEntity);
                pTarget.m_TargetedBy.push_back(iTargetEntity);
                unitState_C->m_iTarget = iTargetEntity;
                unitState_C->traveledDistance = 0;
                unitState_C->goal = goalPosition.localPosition;
                unitState_C->state = State::MOVED;

                counter++;
                if (counter >= amount)
                {
                    break;
                }
            }
        }
    }

    // void MovementSystem::setAttacking(Transform& root, glm::vec3 targetPosition, int amount)
    // {
    //     int counter = 0;
    //     for (auto& childTransform : root.getChildTransforms())
    //     {
    //         auto& unitState_C = engine.componentManager.getComponent<UnitState>(childTransform->entity());
    //         if (unitState_C.state == State::IDLE && counter < amount)
    //         {
    //             unitState_C.state = State::ATTACKING;
    //             counter++;
    //         }
    //     }
    // }
}