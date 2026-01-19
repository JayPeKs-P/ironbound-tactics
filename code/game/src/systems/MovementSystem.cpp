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
#include "engine/rendering/InstanceBuffer.h"
#include "engine/rendering/Model2D.h"
#include "glm/gtx/norm.inl"

using gl3::engine::sceneGraph::Transform;

namespace gl3 {
    MovementSystem::event_t MovementSystem::finishAnimation;
    MovementSystem::event_t MovementSystem::finishedAllAnimations;

    MovementSystem::MovementSystem(engine::Game& game) :
        System(game) {
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

        CombatController::onBeforeAttack.addListener([&](guid_t unit, guid_t target, int amount)
        {
            if (!engine.entityManager.checkIfEntityHasComponent<Unit>(unit, target)) throw(
                "MovementSystem::MovementSystem() onBeforeAttack.addListener missing unit_C");
            auto unit_C = &engine.componentManager.getComponent<Unit>(unit);
            auto& actorRoot = engine.componentManager.getComponent<Transform>(unit);
            auto& targetRoot = engine.componentManager.getComponent<Transform>(target);
            auto& tag = engine.componentManager.getComponent<TagComponent>(unit).value;

            switch (unit_C->category)
            {
            case UnitCategory::INFANTRY:
                {
                    if (tag == Tag::PLAYER)
                    {
                        setMoving(actorRoot, playerPendingPosition, amount, State::IDLE);
                    }
                    else if (tag == Tag::ENEMY)
                    {
                        setMoving(actorRoot, enemyPendingPosition, amount, State::IDLE);
                    }
                    break;
                }
            case UnitCategory::ARCHER:
                {
                    // maybe pass UnitCategory here? or add another State::SHOOTING and make CreateProjectiles() set units to fighting
                    SetAiming(actorRoot, targetRoot, amount, State::IDLE);
                    break;
                }
            }
        });
        // Sets units to Moved if they were flagged with Preparing
        CombatController::onAttack.addListener([&](guid_t unit, guid_t target, int amount)
        {
            if (!engine.entityManager.checkIfEntityHasComponent<Unit>(unit, target)) throw(
                "MovementSystem::MovementSystem() onAttack.addListener missing unit_C");
            auto unit_C = &engine.componentManager.getComponent<Unit>(unit);
            auto& actorRoot = engine.componentManager.getComponent<Transform>(unit);
            auto& targetRoot = engine.componentManager.getComponent<Transform>(target);
            switch (unit_C->category)
            {
            case UnitCategory::INFANTRY:
                {
                    setMoved(actorRoot, targetRoot, amount, State::PREPARING);
                    break;
                }
            case UnitCategory::ARCHER:
                {
                    break;
                }
            }
        });
        CombatController::onAfterAttack.addListener([&](guid_t unused1, guid_t unused2, int unused3)
        {
            // engine.componentManager.forEachComponent<Unit>([&](const Unit& unit_C)
            // {
            //     if (unit_C.category == UnitCategory::INFANTRY)
            //     {
            //         auto& root = engine.componentManager.getComponent<Transform>(unit_C.entity());
            //         SetResetting(root, State::FIGHTING);
            //     }
            // });
        });
    }

    void MovementSystem::moveTo(engine::Game& game, float deltaTime) {
        try
        {
            m_bAllAnimationsFinished = true;
            m_bMoveAnimsFinished = true;
            m_bAttackAnimsFinished = true;
            game.componentManager.forEachComponent<Transform>([&](Transform& transform)
            {
                if (m_bPlayFightAnimation)
                {
                    if (game.componentManager.hasComponent<ProjectileState>(transform.entity()))
                    {
                        if (m_bUpdateProjectileTarget == true)
                        {
                            auto pProjectileState = &game.componentManager.getComponent<ProjectileState>(transform.entity());
                            CheckIfTargetMoved(*pProjectileState);
                        }
                        m_bAttackAnimsFinished = MoveCurved(transform, deltaTime);
                    }
                }
                else
                {
                    if (game.componentManager.hasComponent<UnitState>(transform.entity()))
                    {
                        auto root = transform.getParent();
                        auto unitState_C = &game.componentManager.getComponent<UnitState>(transform.entity());

                        switch (unitState_C->state)
                        {
                        case State::MOVING:
                            {
                                glm::vec3 directionPrep = unitState_C->goal - transform.localPosition;
                                m_bMoveAnimsFinished = moveStraight(transform, directionPrep, deltaTime,
                                                                    State::PREPARING, 3);
                                break;
                            }
                        case State::PREPARING:
                            {
                                break;
                            }
                        case State::MOVED:
                            {
                                glm::vec3 directionMov;
                                if (unitState_C->m_iTarget != engine::ecs::invalidID)
                                {
                                    if (!engine.entityManager.IsAlive(unitState_C->m_iTarget))
                                    {
                                        unitState_C->m_iTarget = engine::ecs::invalidID;
                                        unitState_C->goal = transform.localPosition;
                                        directionMov = {0, 0, 0};

                                        m_bMoveAnimsFinished = moveStraight(transform, directionMov, deltaTime, State::FIGHTING,
                                                                            15);
                                        break;
                                    }
                                    auto pTargetTransform = &engine.componentManager.getComponent<Transform>(
                                        unitState_C->m_iTarget);
                                    directionMov = pTargetTransform->localPosition - transform.localPosition;
                                    unitState_C->goal = pTargetTransform->localPosition;
                                }
                                else
                                {
                                    unitState_C->goal = transform.localPosition;
                                    directionMov = {0, 0, 0};
                                }
                                m_bMoveAnimsFinished = moveStraight(transform, directionMov, deltaTime, State::FIGHTING,
                                                                    15);
                                break;
                            }
                        case::State::AIMING:
                            {
                                m_bMoveAnimsFinished = CreateProjectiles(transform, State::IDLE);
                                break;
                            }
                        case State::FIGHTING:
                            {
                                if (m_bResetUnits)
                                {
                                    SetResetting(transform, State::FIGHTING);
                                    m_bMoveAnimsFinished = false;
                                }
                                break;
                            }
                        case State::RESETTING:
                            {
                                glm::vec3 directionRes = unitState_C->goal - transform.localPosition;
                                m_bMoveAnimsFinished = moveStraight(transform, directionRes, deltaTime, State::IDLE, 3);
                                break;
                            }
                        case State::IDLE:
                            {
                                break;
                            }
                        }
                    }
                }
            });
            if (m_bMoveAnimsFinished && !m_bPlayFightAnimation && !m_bResetUnits)
            {
                m_bPlayFightAnimation = true;
                m_bMoveAnimsFinished = false;
                m_bAttackAnimsFinished = false;
            }
            if (m_bAttackAnimsFinished && m_bPlayFightAnimation && !m_bResetUnits)
            {
                m_bPlayFightAnimation = false;
                m_bMoveAnimsFinished = false;
                m_bResetUnits = true;
                m_bUpdateProjectileTarget = m_bAttackAnimsFinished;
            }
            if (m_bMoveAnimsFinished && m_bAttackAnimsFinished) m_bAllAnimationsFinished = true;
            else m_bAllAnimationsFinished = false;

            if (m_bAllAnimationsFinished)
            {
                countdown -= deltaTime;
            }

            if (countdown <= 0 && m_bAllAnimationsFinished)
            {
                m_bResetUnits = false;
                finishedAllAnimations.invoke(true);
                countdown = 0.5f;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Unhandled exception in MoveTo function" << e.what() << std::endl;
        }
    }

    bool MovementSystem::moveStraight(Transform& transform, glm::vec3 direction, float deltatime, State endState,
                                      float delay) {
        bool bFinished = true;
        float distanceToGoal = glm::length(direction);
        auto& unitState_C = engine.componentManager.getComponent<UnitState>(transform.entity());
        auto speed = unitState_C.movementSpeed;
        if (unitState_C.state == State::RESETTING) speed *= 2.5;
        auto directionNormalized = glm::normalize(direction);
        auto directionAngle = glm::degrees(std::atan2(directionNormalized.x, directionNormalized.y));
        auto iUnit = transform.entity();

        auto pUvOffset = &engine.componentManager.getComponent<UvOffset>(iUnit);
        if ( (directionAngle < 45) || 315 <= directionAngle)
        {
            pUvOffset->v = LEFT;
        }
        else if (directionAngle < 135)
        {
            pUvOffset->v = RIGHT;
        }
        else if (directionAngle < 225)
        {
           pUvOffset->v = UP;
        }
        else
        {
            pUvOffset->v = DOWN;
        }

        if (distanceToGoal - speed * deltatime > 0.05)
        {
            transform.localPosition += speed * deltatime * directionNormalized;
            unitState_C.traveledDistance += deltatime * speed;
            bFinished = false;
        }
        else
        {
            unitState_C.oldPos = transform.localPosition;
            unitState_C.state = endState;
            unitState_C.traveledDistance = 0;
            unitState_C.m_iTarget = engine::ecs::invalidID;
            finishAnimation.invoke(delay);
            if (unitState_C.state == State::IDLE)
            {
                transform.localPosition = transform.getParent()->localPosition + unitState_C.relativeVec;
                pUvOffset->v = pUvOffset->originalV;
            }
        }
        return bFinished;
    }

    bool MovementSystem::CheckIfTargetMoved(ProjectileState& projectile) {
        auto pTargetTransform = &engine.componentManager.getComponent<Transform>(projectile.m_iTarget);
        auto distance = glm::distance2(projectile.endPos, pTargetTransform->localPosition);
        if (distance > 0.05)
        {
            projectile.endPos = pTargetTransform->localPosition;
        }
        return true;
    }

    bool MovementSystem::CreateProjectiles(Transform& transform, State endState) {

        auto pUnitState_C = &engine.componentManager.getComponent<UnitState>(transform.entity());

        auto pTargetTransform = &engine.componentManager.getComponent<Transform>(pUnitState_C->m_iTarget);
        int iRootProjectile = engine::ecs::invalidID;
        engine.componentManager.forEachComponent<Projectile>([&](Projectile& projectile)
        {
            if (iRootProjectile != engine::ecs::invalidID) return;
            if (projectile.entity() == projectile.m_iRefComponent)
            {
                iRootProjectile = projectile.m_iRefComponent;
            }
        });
        auto pRootArrowTransform_C = &engine.componentManager.getComponent<Transform>(iRootProjectile);

        auto pProjectile_E = &engine.entityManager.createEntity();

        auto pProjectileTransform_C = &pProjectile_E->addComponent<Transform>(
            pRootArrowTransform_C, transform.localPosition, transform.localZRotation, transform.localScale);
        auto pProjectileState_C = &pProjectile_E->addComponent<ProjectileState>();
        pProjectileState_C->startPos = pProjectileTransform_C->localPosition;
        pProjectileState_C->lastPos = pProjectileTransform_C->localPosition;
        pProjectileState_C->endPos = pTargetTransform->localPosition;
        pProjectileState_C->m_iTarget = pUnitState_C->m_iTarget;

        float direction = pTargetTransform->localPosition.x - pProjectileTransform_C->localPosition.x;
        if (direction < 0.0f)
        {
            pProjectileTransform_C->localZRotation = 135;
        }
        else
        {
            pProjectileTransform_C->localZRotation = 45;
        }

        pUnitState_C->state = endState;
        pUnitState_C->m_iTarget = engine::ecs::invalidID;
        auto pProjectileAmount_C = &engine.componentManager.getComponent<InstanceAmount>(iRootProjectile);
        pProjectileAmount_C->value++;

        return false;
    }

    bool MovementSystem::DeleteProjectile(guid_t ID) {
        engine.componentManager.forEachComponent<Projectile>([&](Projectile& projectile)
        {
            if (projectile.m_iRefComponent == projectile.entity()) return;
            engine.componentManager.removeComponent<Projectile>(projectile.entity());
        });
        auto projectile = engine.entityManager.getEntity(ID);
        engine.entityManager.deleteEntity(projectile);
        return true;
    }

    bool MovementSystem::ShootProjectiles(Transform& transform, float deltatime, State endState) {
        return true;

        if (transform.getChildTransforms().empty())
        {
            engine.entityManager.createEntity();
        }
    }

    bool MovementSystem::MoveCurved(Transform& projectileTransform, float deltatime) {
        auto& projectileState_C = engine.componentManager.getComponent<ProjectileState>(projectileTransform.entity());

        float flightTime = 0.9f;
        projectileState_C.elapsedTime += deltatime;
        float timeNorm = glm::clamp(projectileState_C.elapsedTime / flightTime, 0.0f, 1.0f);

        const auto startPos = projectileState_C.startPos;
        const auto endPos = projectileState_C.endPos;

        glm::vec3 baseForward = glm::normalize(endPos - startPos);
        glm::vec3 worldUp = glm::vec3(0, 1, 0);
        glm::vec3 right = glm::normalize(glm::cross(worldUp, baseForward));
        glm::vec3 baseUp = glm::normalize(glm::cross(baseForward, right));

        glm::vec3 horizontalDirection = endPos - startPos;
        float xEnd = glm::dot(horizontalDirection, baseForward);
        float maxHeight = 0.5f;


        glm::vec3 lastPos = projectileState_C.lastPos;
        float xDist = glm::mix(0.0f, xEnd, timeNorm);
        float yDist = maxHeight * 4.0f * timeNorm * (1.0f - timeNorm);
        projectileTransform.localPosition = startPos + baseForward * xDist + baseUp * yDist;

        glm::vec2 rotation = glm::vec2(projectileTransform.localPosition.x - lastPos.x,
                                        projectileTransform.localPosition.y - lastPos.y);
        if (glm::length(rotation) > 0.0001f)
        {
            projectileTransform.localZRotation = glm::degrees(std::atan2(rotation.y, rotation.x));
        }
        projectileState_C.lastPos = projectileTransform.localPosition;

        if (timeNorm >= 1.0f)
        {
            DeleteProjectile(projectileTransform.entity());
            return true;
        }
        return false;
    }

    void MovementSystem::moveCurved(Transform& root, glm::vec3 goal, float compression, float deltatime) {
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
            if (unitState_C.state != State::MOVING) continue;
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
            }
            else
            {
                unitState_C.state = State::IDLE;
                finishAnimation.invoke(true);
            }
        }
    }

    void MovementSystem::setMoving(Transform& root, glm::vec3 goalPosition, int amount, State initialState) {
        int counter = 0;
        for (auto& childTransform : root.getChildTransforms())
        {
            auto& unitState_C = engine.componentManager.getComponent<UnitState>(childTransform->entity());
            if (unitState_C.state != initialState) continue;

            unitState_C.relativeVec = childTransform->localPosition - root.localPosition;
            auto directionGlobal = goalPosition - root.localPosition;
            unitState_C.traveledDistance = 0;
            unitState_C.goal = childTransform->localPosition + directionGlobal;
            unitState_C.state = State::MOVING;

            counter++;
            if (counter >= amount) break;
        }
    }

    void MovementSystem::setMoved(Transform& root, Transform& goalPosition, int amount, State initialState) {
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
                    auto unitStateTarget_C = &engine.componentManager.getComponent<UnitState>(
                        childTransformTarget->entity());
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
                auto pTarget = &engine.componentManager.getComponent<UnitState>(iTargetEntity);
                pTarget->m_TargetedBy.push_back(iTargetEntity);
                unitState_C->m_iTarget = iTargetEntity;
                unitState_C->traveledDistance = 0;
                unitState_C->goal = goalPosition.localPosition;
                unitState_C->state = State::MOVED;

                counter++;
                if (counter >= amount) break;
            }
        }
    }

    void MovementSystem::SetResetting(Transform& unitTransform, State initialState) const {
        if (!engine.entityManager.checkIfEntityHasComponent<UnitState>(unitTransform.entity())) return;
        auto unitState_C = &engine.componentManager.getComponent<UnitState>(unitTransform.entity());
        auto parrentTransform = unitTransform.getParent();
        if (unitState_C->state == initialState)
        {
            unitState_C->state = State::RESETTING;
            unitState_C->goal = parrentTransform->localPosition + unitState_C->relativeVec;
            unitState_C->m_TargetedBy.clear();
        }
    }

    void MovementSystem::SetAiming(Transform& root, Transform& targetPosition, int amount, State initialState) {
        int counter = 0;
        for (auto& childTransform : root.getChildTransforms())
        {
            auto unitState_C = &engine.componentManager.getComponent<UnitState>(childTransform->entity());
            if (unitState_C->state == initialState)
            {
                size_t currentSmalestSize = 5000;
                guid_t iTargetEntity = engine::ecs::invalidID;
                for (auto childTransformTarget : targetPosition.getChildTransforms())
                {
                    auto unitStateTarget_C = &engine.componentManager.getComponent<UnitState>(
                        childTransformTarget->entity());
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
                auto pTarget = &engine.componentManager.getComponent<UnitState>(iTargetEntity);
                pTarget->m_TargetedBy.push_back(iTargetEntity);
                unitState_C->m_iTarget = iTargetEntity;
                unitState_C->state = State::AIMING;

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
