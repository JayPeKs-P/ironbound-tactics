//
// Created by julia on 22/07/2025.
//
#pragma once
#include "engine/sceneGraph/Transform.h"
#include "engine/ecs/System.h"
#include "engine/Events.h"
#include "engine/Game.h"
#include "engine/Game.h"


struct UnitState;
struct ProjectileState;
enum class State;
using gl3::engine::sceneGraph::Transform;
namespace gl3 {

class MovementSystem: public engine::ecs::System{
public:
    using event_t = gl3::engine::events::Event<MovementSystem, float>;
    static event_t finishedAllAnimations;

    explicit MovementSystem(engine::Game &game);
    void Animate(float deltatime);
    void update(engine::Game &game, Transform* root);
    bool moveStraight(Transform& transform, glm::vec3 direction, float deltatime, State endState) const;
    bool CheckIfTargetMoved(ProjectileState& projectileState_C) const;
    bool CreateProjectiles(const Transform& transform, State endState, int iDelay) const;
    bool DeleteProjectile(guid_t ID);
    bool MoveCurved(Transform& projectileTransform, float deltatime);
private:
    template<typename C>
    void HelperTargetValidity(C* pComponent);
    guid_t HelperGetTargetInstance(Transform& rootTargetTransform_C) const;
    void UpdateEndPosition(UnitState* pUnitState_C);
    void HandleMovementAnimation(Transform& transform, float deltaTime);
    void HandleAttackAnimation(Transform& transform, float deltaTime);
    void PrepareMoving(UnitState* pUnitState_C, Transform& rootUnitTransform_C, Transform& rootTargetTransform_C);
    void PrepareAttack(UnitState* pUnitState_C,guid_t iRootActor, Transform& rootTargetTransform_C) const;
    void SetUnitState(Transform& rootActorTransform_C, Transform& rootTargetTransform_C, int iAmount, State initialActorState, State endActorState, int
                                      iDelay = 0);
    void SetResetting(Transform& unitTransform, State initialState) const;
    void AtomicCheckAnimationFinished();
    void UpdateAnimationStage();
    void FinishAnimationPhase(float deltaTime);


    glm::vec3 movementVector;
    glm::vec3 playerPendingPosition = glm::vec3(-0.2, -0.3, 0.0);
    glm::vec3 enemyPendingPosition = glm::vec3(0.0, 0.3, 0.0);
    bool m_bAllAnimationsFinished = false;
    bool m_bMoveAnimsFinished = false;
    bool m_bUpdateProjectileTarget = true;
    bool m_bPlayFightAnimation = false;
    bool m_bResetUnits = false;
    bool m_bAttackAnimsFinished = false;

    float countdown = 0.5f;
};

} // gl3