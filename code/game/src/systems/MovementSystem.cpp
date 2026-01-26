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
    MovementSystem::event_t MovementSystem::finishedAllAnimations;

    MovementSystem::MovementSystem(engine::Game& game) :
        System(game) {
        CombatController::onBeforeDamageStep.addListener([&]()
        {
            Animate(engine, engine.getDeltaTime());
        });
        auto pPlayerPendingRoot_E = &engine.entityManager.createEntity();
        auto pEnemyPendingRoot_E = &engine.entityManager.createEntity();
        auto& playerPendingTransform_C = pPlayerPendingRoot_E->addComponent<Transform>(engine.origin, playerPendingPosition);
        auto& enemyPendingTransform_C = pEnemyPendingRoot_E->addComponent<Transform>(engine.origin, playerPendingPosition);
#ifdef DEBUG_MODE
#endif

        CombatController::onBeforeAttack.addListener([&](guid_t unit, guid_t target, int amount)
        {
            if (!engine.entityManager.checkIfEntityHasComponent<Unit>(unit, target))
                throw(
                    "MovementSystem::MovementSystem() onBeforeAttack.addListener missing unit_C");
            auto pUnit_C = &engine.componentManager.getComponent<Unit>(unit);
            auto& rootActorTransform_C = engine.componentManager.getComponent<Transform>(unit);
            auto& rootTransformTarget_C = engine.componentManager.getComponent<Transform>(target);
            auto& tagActor_C = engine.componentManager.getComponent<TagComponent>(unit).value;
            auto& tagTarget_C = engine.componentManager.getComponent<TagComponent>(target).value;

            switch (pUnit_C->category) {
            case UnitCategory::INFANTRY: {
                if (tagActor_C == tagTarget_C) {
                    // const guid_t iTarget = HelperGetTargetInstance(rootTransformTarget_C);
                    // auto& instanceTargetTransform_C = engine.componentManager.getComponent<Transform>(iTarget);
                    SetUnitState(rootActorTransform_C, rootTransformTarget_C, amount, State::IDLE, State::MOVE_TO_SIEGE);
                }
                if (tagActor_C == Tag::PLAYER) {
                    SetUnitState(rootActorTransform_C, playerPendingTransform_C, amount, State::IDLE, State::MOVING);
                    // setMoving(rootActorTransform_C, playerPendingPosition, amount, State::IDLE);
                }
                else if (tagActor_C == Tag::ENEMY) {
                    SetUnitState(rootActorTransform_C, enemyPendingTransform_C, amount, State::IDLE, State::MOVING);
                    // setMoving(rootActorTransform_C, enemyPendingPosition, amount, State::IDLE);
                }
                break;
            }
            case UnitCategory::ARCHER: {
                // maybe pass UnitCategory here? or add another State::SHOOTING and make CreateProjectiles() set units to fighting
                SetUnitState(rootActorTransform_C, rootTransformTarget_C, amount, State::IDLE, State::AIMING, pUnit_C->speed);
                // SetAiming(rootActorTransform_C, rootTransformTarget_C, amount, State::IDLE, pUnit_C->speed);
                break;
            }
            case UnitCategory::CATAPULT: {
                SetUnitState(rootActorTransform_C, rootTransformTarget_C, amount, State::IDLE, State::AIMING, pUnit_C->speed);
                // SetAiming(rootActorTransform_C, rootTransformTarget_C, amount, State::IDLE, pUnit_C->speed);
                break;
            }
            }
        });
        // Sets units to Moved if they were flagged with Preparing
        CombatController::onAttack.addListener([&](guid_t unit, guid_t target, int amount)
        {
            if (!engine.entityManager.checkIfEntityHasComponent<Unit>(unit, target))
                throw(
                    "MovementSystem::MovementSystem() onAttack.addListener missing unit_C");
            auto unit_C = &engine.componentManager.getComponent<Unit>(unit);
            auto& rootActorTransform_C = engine.componentManager.getComponent<Transform>(unit);
            auto& rootTargetTransform_C = engine.componentManager.getComponent<Transform>(target);
            switch (unit_C->category) {
            case UnitCategory::INFANTRY: {
                SetUnitState(rootActorTransform_C, rootTargetTransform_C, amount, State::PREPARING, State::MOVED);
                // setMoved(rootActorTransform_C, rootTargetTransform_C, amount, State::PREPARING);
                break;
            }
            }
        });
        CombatController::turnEnd.addListener([&]()
        {
            engine.componentManager.forEachComponent<ProjectileState>([&](ProjectileState& projectileState_C)
            {
                projectileState_C.m_iDelayTurns--;
            });
            engine.componentManager.forEachComponent<Projectile>([&](Projectile& projectile)
            {
                for (int i = 1; i < projectile.m_ActiveProjectileList.size(); i++) {
                    projectile.m_ActiveProjectileList[i - 1] += projectile.m_ActiveProjectileList[i];
                    projectile.m_ActiveProjectileList[i] = 0;
                }
#ifdef _DEBUG
                assert(projectile.m_ActiveProjectileList[MIN_SPEED_VALUE - 1] == 0);
#endif
            });
        });
    }

    void MovementSystem::Animate(engine::Game& game, float deltaTime) {
        try {
            m_bAllAnimationsFinished = true;
            m_bMoveAnimsFinished = true;
            m_bAttackAnimsFinished = true;
            game.componentManager.forEachComponent<Transform>([&](Transform& transform)
            {
                if (m_bPlayFightAnimation) {
                    if (!game.componentManager.hasComponent<ProjectileState>(transform.entity())) return;
                    auto pProjectileState = &game.componentManager.getComponent<ProjectileState>(transform.entity());
                    if (pProjectileState->m_iDelayTurns > MIN_SPEED_VALUE) return;

                    if (!pProjectileState->m_bUpdated) {
                        HelperTargetValidity(pProjectileState);
                        pProjectileState->m_bUpdated = CheckIfTargetMoved(*pProjectileState);
                    }
                    MoveCurved(transform, deltaTime);
                }
                else {
                    if (!game.componentManager.hasComponent<UnitState>(transform.entity())) return;
                    auto pUnitState_C = &game.componentManager.getComponent<UnitState>(transform.entity());

                    switch (pUnitState_C->state) {
                    case State::MOVING: {
                        glm::vec3 directionPrep = pUnitState_C->endPos - transform.localPosition;
                        bool bFinished = moveStraight(transform, directionPrep, deltaTime, State::PREPARING);
                        if (m_bMoveAnimsFinished) m_bMoveAnimsFinished = bFinished;
                        break;
                    }
                    case State::MOVE_TO_SIEGE: {
                        // glm::vec3 directionPrep = pUnitState_C->endPos - transform.localPosition;
                        // bool bFinished = moveStraight(transform, directionPrep, deltaTime, State::USING);
                        // if (m_bMoveAnimsFinished) m_bMoveAnimsFinished = bFinished;
                        glm::vec3 directionMov = {0, 0, 0};

                        HelperTargetValidity(pUnitState_C); // make return bool to replace ↓

                        const bool bValidTargetID = pUnitState_C->m_iTarget != engine::ecs::invalidID;
                        if (bValidTargetID) {
                            auto pTargetTransform = &engine.componentManager.getComponent<Transform>(
                                pUnitState_C->m_iTarget);
                            directionMov = pTargetTransform->localPosition - transform.localPosition;
                            pUnitState_C->endPos = pTargetTransform->localPosition;
                        }
                        bool bFinished = moveStraight(transform, directionMov, deltaTime, State::USING);
                        if (m_bMoveAnimsFinished) m_bMoveAnimsFinished = bFinished;
                        break;
                    }
                    case State::MOVED: {
                        glm::vec3 directionMov = {0, 0, 0};

                        HelperTargetValidity(pUnitState_C); // make return bool to replace ↓

                        const bool bValidTargetID = pUnitState_C->m_iTarget != engine::ecs::invalidID;
                        if (bValidTargetID) {
                            auto pTargetTransform = &engine.componentManager.getComponent<Transform>(
                                pUnitState_C->m_iTarget);
                            directionMov = pTargetTransform->localPosition - transform.localPosition;
                            pUnitState_C->endPos = pTargetTransform->localPosition;
                        }
                        bool bFinished = moveStraight(transform, directionMov, deltaTime, State::FIGHTING);
                        if (m_bMoveAnimsFinished) m_bMoveAnimsFinished = bFinished;
                        break;
                    }
                    case State::FIGHTING: {
                        if (m_bResetUnits) {
                            SetResetting(transform, State::FIGHTING);
                            m_bMoveAnimsFinished = false;
                        }
                        break;
                    }
                    case State::RESETTING: {
                        glm::vec3 directionRes = pUnitState_C->endPos - transform.localPosition;
                        bool bFinished = moveStraight(transform, directionRes, deltaTime, State::IDLE);
                        if (m_bMoveAnimsFinished) m_bMoveAnimsFinished = bFinished;
                        break;
                    }
                    }
                }
            });

            engine.componentManager.forEachComponent<Projectile>([&](const Projectile& projectile_C)
            {
                if (!m_bAttackAnimsFinished) return;
                if (projectile_C.m_ActiveProjectileList[MIN_SPEED_VALUE] > 0) m_bAttackAnimsFinished = false;
            });

            if (m_bMoveAnimsFinished && !m_bPlayFightAnimation && !m_bResetUnits) {
                m_bPlayFightAnimation = true;
                m_bMoveAnimsFinished = false;
                m_bAttackAnimsFinished = false;
            }
            if (m_bAttackAnimsFinished && m_bPlayFightAnimation && !m_bResetUnits) {
                m_bPlayFightAnimation = false;
                m_bMoveAnimsFinished = false;
                m_bResetUnits = true;
                m_bUpdateProjectileTarget = m_bAttackAnimsFinished;
            }
            if (m_bMoveAnimsFinished && m_bAttackAnimsFinished) m_bAllAnimationsFinished = true;
            else m_bAllAnimationsFinished = false;

            if (m_bAllAnimationsFinished) {
                countdown -= deltaTime;
            }

            if (countdown <= 0 && m_bAllAnimationsFinished) {
                m_bResetUnits = false;
                finishedAllAnimations.invoke(true);
                countdown = 0.5f;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Unhandled exception in MoveTo function" << e.what() << std::endl;
        }
    }

    template <typename C>
    void MovementSystem::HelperTargetValidity(C* pComponent) {
        const bool bValidTargetID = pComponent->m_iTarget != engine::ecs::invalidID;
        const bool bEntityAlive = engine.entityManager.IsAlive(pComponent->m_iTarget);
        if (!bValidTargetID || !bEntityAlive) {
            pComponent->m_iTarget = engine::ecs::invalidID;
            pComponent->endPos = pComponent->startPos;

            engine.componentManager.forEachComponent<UnitState>([&](UnitState& unitState)
            {
                if (pComponent->m_iTarget != engine::ecs::invalidID) return;
                if (unitState.m_iParentEntity != pComponent->m_iTargetParentEntity) return;

                guid_t iNewTarget = unitState.entity();
                pComponent->m_iTarget = iNewTarget;
                auto pTargetTransform = &engine.componentManager.getComponent<Transform>(pComponent->m_iTarget);
                pComponent->endPos = pTargetTransform->localPosition;
            });
        }
    }

    template void MovementSystem::HelperTargetValidity<UnitState>(UnitState*);
    template void MovementSystem::HelperTargetValidity<ProjectileState>(ProjectileState*);

    bool MovementSystem::moveStraight(Transform& transform, glm::vec3 direction, float deltatime,
                                      State endState) const {
        bool bFinished = true;
        float distanceToGoal = glm::length(direction);
        auto& unitState_C = engine.componentManager.getComponent<UnitState>(transform.entity());
        auto speed = unitState_C.movementSpeed;
        if (unitState_C.state == State::RESETTING) speed *= 2.5;
        auto directionNormalized = glm::normalize(direction);
        auto directionAngle = glm::degrees(std::atan2(directionNormalized.x, directionNormalized.y));
        auto iUnit = transform.entity();

        auto pUvOffset = &engine.componentManager.getComponent<UvOffset>(iUnit);
        if ((directionAngle < 45) || 315 <= directionAngle) {
            pUvOffset->v = LEFT;
        }
        else if (directionAngle < 135) {
            pUvOffset->v = RIGHT;
        }
        else if (directionAngle < 225) {
            pUvOffset->v = UP;
        }
        else {
            pUvOffset->v = DOWN;
        }

        if (distanceToGoal - speed * deltatime > 0.05) {
            transform.localPosition += speed * deltatime * directionNormalized;
            // unitState_C.traveledDistance += deltatime * speed;
            bFinished = false;
        }
        else {
            unitState_C.oldPos = transform.localPosition;
            unitState_C.state = endState;
            // unitState_C.traveledDistance = 0;
            unitState_C.m_iTarget = engine::ecs::invalidID;
            if (unitState_C.state == State::IDLE) {
                transform.localPosition = transform.getParent()->localPosition + unitState_C.relativeVec;
                pUvOffset->v = pUvOffset->originalV;
            }
        }
        return bFinished;
    }

    bool MovementSystem::CheckIfTargetMoved(ProjectileState& projectileState_C) const {
        if (projectileState_C.m_iTarget == engine::ecs::invalidID) return true;

        auto pTargetTransform = &engine.componentManager.getComponent<Transform>(projectileState_C.m_iTarget);
        auto distanceDifference = glm::distance(projectileState_C.endPos, pTargetTransform->localPosition);
        if (distanceDifference > 0.05) {
            projectileState_C.endPos = pTargetTransform->localPosition;

            auto travelDistance = glm::distance(projectileState_C.endPos, projectileState_C.startPos);
            float speed = projectileState_C.m_fProjectileSpeed;
            projectileState_C.m_fFlightTime = glm::max(travelDistance / speed, 0.05f);
            projectileState_C.elapsedTime = 0.0f;
        }
        return true;
    }

    bool MovementSystem::CreateProjectiles(const Transform& transform, State endState, int iDelay) const {
        auto pUnitState_C = &engine.componentManager.getComponent<UnitState>(transform.entity());

        auto pTargetTransform = &engine.componentManager.getComponent<Transform>(pUnitState_C->m_iTarget);
        int iRootProjectile = engine::ecs::invalidID;
        engine.componentManager.forEachComponent<Projectile>([&](Projectile& projectile)
        {
            if (iRootProjectile != engine::ecs::invalidID) return;
            if (projectile.entity() == projectile.m_iRefComponent) {
                iRootProjectile = projectile.m_iRefComponent;
                projectile.m_ActiveProjectileList[iDelay]++;
            }
        });
        auto pRootArrowTransform_C = &engine.componentManager.getComponent<Transform>(iRootProjectile);

        auto pProjectile_E = &engine.entityManager.createEntity();

        auto pProjectileTransform_C = &pProjectile_E->addComponent<Transform>(
            pRootArrowTransform_C, transform.localPosition, transform.localZRotation, transform.localScale);
        auto pProjectileState_C = &pProjectile_E->addComponent<ProjectileState>(iDelay);
        pProjectileState_C->startPos = pProjectileTransform_C->localPosition;
        pProjectileState_C->lastPos = pProjectileTransform_C->localPosition;
        pProjectileState_C->endPos = pTargetTransform->localPosition;
        pProjectileState_C->m_iTarget = pUnitState_C->m_iTarget;
        pProjectileState_C->m_iDelayTurns = iDelay;
        float speed = pProjectileState_C->m_fProjectileSpeed;
        auto distance = glm::distance(pProjectileState_C->startPos, pProjectileState_C->endPos);
        pProjectileState_C->m_fFlightTime = glm::max(distance / speed, 0.05f);

        float direction = pTargetTransform->localPosition.x - pProjectileTransform_C->localPosition.x;
        if (direction < 0.0f) {
            pProjectileTransform_C->localZRotation = 135;
        }
        else {
            pProjectileTransform_C->localZRotation = 45;
        }

        pUnitState_C->state = endState;
        pUnitState_C->m_iTarget = engine::ecs::invalidID;
        auto pProjectileAmount_C = &engine.componentManager.getComponent<InstanceAmount>(iRootProjectile);
        pProjectileAmount_C->value++;

        return false;
    }

    bool MovementSystem::DeleteProjectile(guid_t ID) {
        engine.PlaySound("retro_impact_colorful_01.wav");
        auto projectile = engine.entityManager.getEntity(ID);
        engine.entityManager.deleteEntity(projectile);
        int iRootProjectile_E = engine.componentManager.getComponent<Transform>(ID).getParent()->entity();
        auto pRootProjectile_C = &engine.componentManager.getComponent<Projectile>(iRootProjectile_E);
        pRootProjectile_C->m_ActiveProjectileList[MIN_SPEED_VALUE]--;
        return true;
    }

    bool MovementSystem::MoveCurved(Transform& projectileTransform, float deltatime) {
        auto& projectileState_C = engine.componentManager.getComponent<ProjectileState>(projectileTransform.entity());

        float flightTime = projectileState_C.m_fFlightTime;
        projectileState_C.elapsedTime += deltatime;
        float timeNorm = glm::clamp(projectileState_C.elapsedTime / flightTime, 0.0f, 1.0f);

        const auto startPos = projectileState_C.startPos;
        const auto endPos = projectileState_C.endPos;

        glm::vec3 baseForward = glm::normalize(endPos - startPos);
        glm::vec3 worldUp = glm::vec3(0, 1, 0);
        glm::vec3 right = glm::normalize(glm::cross(worldUp, baseForward));
        glm::vec3 baseUp = glm::normalize(glm::cross(baseForward, right));

        glm::vec3 horizontalDirection = endPos - startPos;

        if (glm::length(horizontalDirection) < 0.01f) {
            DeleteProjectile(projectileTransform.entity());
            return true;
        }

        float xEnd = glm::dot(horizontalDirection, baseForward);
        float maxHeight = 1.0f;
        if (flightTime < 0.25) maxHeight = 0.001;


        glm::vec3 lastPos = projectileState_C.lastPos;
        float xDist = glm::mix(0.0f, xEnd, timeNorm);
        float yDist = maxHeight * 2.0 * timeNorm * (1.0f - timeNorm);
        projectileTransform.localPosition = startPos + baseForward * xDist + baseUp * yDist;

        glm::vec2 rotation = glm::vec2(projectileTransform.localPosition.x - lastPos.x,
                                       projectileTransform.localPosition.y - lastPos.y);
        if (glm::length(rotation) > 0.0001f) {
            projectileTransform.localZRotation = glm::degrees(std::atan2(rotation.y, rotation.x));
        }
        projectileState_C.lastPos = projectileTransform.localPosition;

        if (timeNorm >= 1.0f) {
            DeleteProjectile(projectileTransform.entity());
            return true;
        }
        return false;
    }

    guid_t MovementSystem::HelperGetTargetInstance(Transform& rootTargetTransform_C) const {
        size_t currentSmallestSize = 5000;
        guid_t iTargetEntity = engine::ecs::invalidID;
        for (auto childTransformTarget : rootTargetTransform_C.getChildTransforms()) {
            auto pUnitStateTarget_C = &engine.componentManager.getComponent<UnitState>(
                childTransformTarget->entity());
            if (pUnitStateTarget_C->m_TargetedBy.empty()) {
                iTargetEntity = childTransformTarget->entity();
                break;
            }
            if (currentSmallestSize > pUnitStateTarget_C->m_TargetedBy.size()) {
                currentSmallestSize = pUnitStateTarget_C->m_TargetedBy.size();
                iTargetEntity = childTransformTarget->entity();
            }
        }
        return iTargetEntity;
    }

    void MovementSystem::PrepareMoving(UnitState* pUnitState_C, Transform& rootUnitTransform_C, Transform& rootTargetTransform_C  ) {
        auto directionGlobal = rootTargetTransform_C.localPosition - rootUnitTransform_C.localPosition;

        // pUnitState_C->traveledDistance = 0;
        pUnitState_C->endPos = pUnitState_C->startPos + directionGlobal;
    }

    void MovementSystem::PrepareAttack(UnitState* pUnitState_C,guid_t iRootActor, Transform& rootTargetTransform_C) const {
        const guid_t iTargetEntity = HelperGetTargetInstance(rootTargetTransform_C);

        pUnitState_C->m_iParentEntity = iRootActor;
        pUnitState_C->m_iTarget = iTargetEntity;
        pUnitState_C->m_iTargetParentEntity = rootTargetTransform_C.entity();
        if (iTargetEntity != engine::ecs::invalidID) {
            auto pTarget = &engine.componentManager.getComponent<UnitState>(iTargetEntity);
            pTarget->m_TargetedBy.push_back(pUnitState_C->entity());
        }
    }

    void MovementSystem::SetUnitState(Transform& rootActorTransform_C, Transform& rootTargetTransform_C,
                                        int iAmount, State initialActorState, State endActorState, const
                                        int iDelay) {
        int counter = 0;
        for (auto& childTransform : rootActorTransform_C.getChildTransforms()) {
            auto pUnitState_C = &engine.componentManager.getComponent<UnitState>(childTransform->entity());
            if (pUnitState_C->state != initialActorState) continue;
            pUnitState_C->state = endActorState;
            switch (endActorState) {
                case State::MOVING: {
                    PrepareMoving(pUnitState_C, rootActorTransform_C, rootTargetTransform_C);
                    break;
                }
                case State::MOVED: {
                    PrepareAttack(pUnitState_C, rootActorTransform_C.entity(), rootTargetTransform_C);
                    break;
                }
                case State::AIMING: {
                    PrepareAttack(pUnitState_C, rootActorTransform_C.entity(), rootTargetTransform_C);
                    CreateProjectiles(*childTransform, State::IDLE, iDelay);
                    break;
                }
                case State::MOVE_TO_SIEGE: {
                    PrepareAttack(pUnitState_C, rootActorTransform_C.entity(), rootTargetTransform_C);
                    break;
                }
            }
            counter++;
            if (counter >= iAmount) break;
        }
    }

    // void MovementSystem::setMoving(Transform& rootUnitTransform_C, glm::vec3 goalPosition, int amount,
    //                                State initialActorState) {
    //     int counter = 0;
    //     for (auto& childTransform : rootUnitTransform_C.getChildTransforms()) {
    //         auto pUnitState_C = &engine.componentManager.getComponent<UnitState>(childTransform->entity());
    //         if (pUnitState_C->state != initialActorState) continue;
    //         pUnitState_C->state = State::MOVING;
    //
    //         auto directionGlobal = goalPosition - rootUnitTransform_C.localPosition;
    //
    //         // pUnitState_C->traveledDistance = 0;
    //         pUnitState_C->endPos = childTransform->localPosition + directionGlobal;
    //
    //         counter++;
    //         if (counter >= amount) break;
    //     }
    // }
    //
    // void MovementSystem::setMoved(Transform& rootActorTransform_C, Transform& rootTargetTransform_C, int iAmount,
    //                               State initialActorState) {
    //     int counter = 0;
    //     for (auto& childTransform : rootActorTransform_C.getChildTransforms()) {
    //         auto pUnitState_C = &engine.componentManager.getComponent<UnitState>(childTransform->entity());
    //         if (pUnitState_C->state != initialActorState) continue;
    //         pUnitState_C->state = State::MOVED;
    //
    //         const guid_t iTargetEntity = HelperGetTargetInstance(rootTargetTransform_C);
    //
    //         pUnitState_C->m_iParentEntity = rootActorTransform_C.entity();
    //         pUnitState_C->m_iTarget = iTargetEntity;
    //         pUnitState_C->m_iTargetParentEntity = rootTargetTransform_C.entity();
    //         if (iTargetEntity != engine::ecs::invalidID) {
    //             auto pTarget = &engine.componentManager.getComponent<UnitState>(iTargetEntity);
    //             pTarget->m_TargetedBy.push_back(pUnitState_C->entity());
    //         }
    //
    //         // pUnitState_C->traveledDistance = 0;
    //         // pUnitState_C->endPos = rootTargetTransform_C.localPosition;
    //
    //         counter++;
    //         if (counter >= iAmount) break;
    //     }
    // }

    // void MovementSystem::SetAiming(Transform& rootActorTransform_C, Transform& rootTargetTransform_C, int iAmount,
    //                                State intitialActorState, int iDelay) {
    //     int counter = 0;
    //     for (auto& childTransform : rootActorTransform_C.getChildTransforms()) {
    //         auto pUnitState_C = &engine.componentManager.getComponent<UnitState>(childTransform->entity());
    //         if (pUnitState_C->state != intitialActorState) continue;
    //         pUnitState_C->state = State::AIMING;
    //
    //         const guid_t iTargetEntity = HelperGetTargetInstance(rootTargetTransform_C);
    //
    //         pUnitState_C->m_iParentEntity = rootActorTransform_C.entity();
    //         pUnitState_C->m_iTarget = iTargetEntity;
    //         pUnitState_C->m_iTargetParentEntity = rootTargetTransform_C.entity();
    //
    //         auto pTarget = &engine.componentManager.getComponent<UnitState>(iTargetEntity);
    //         pTarget->m_TargetedBy.push_back(pUnitState_C->entity());
    //
    //
    //         CreateProjectiles(*childTransform, State::IDLE, iDelay);
    //         counter++;
    //         if (counter >= iAmount) {
    //             break;
    //         }
    //     }
    // }

    void MovementSystem::SetResetting(Transform& unitTransform, State initialState) const {
        if (!engine.entityManager.checkIfEntityHasComponent<UnitState>(unitTransform.entity())) return;
        auto unitState_C = &engine.componentManager.getComponent<UnitState>(unitTransform.entity());
        auto parrentTransform = unitTransform.getParent();
        if (unitState_C->state == initialState) {
            unitState_C->state = State::RESETTING;
            unitState_C->endPos = parrentTransform->localPosition + unitState_C->relativeVec;
            unitState_C->m_TargetedBy.clear();
        }
    }
}
