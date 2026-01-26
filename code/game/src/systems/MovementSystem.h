//
// Created by julia on 22/07/2025.
//
#pragma once
#include "engine/sceneGraph/Transform.h"
#include "engine/ecs/System.h"
#include "engine/Events.h"
#include "engine/Game.h"
#include "engine/Game.h"


struct ProjectileState;
enum class State;
using gl3::engine::sceneGraph::Transform;
namespace gl3 {

class MovementSystem: public engine::ecs::System{
public:
    using event_t = gl3::engine::events::Event<MovementSystem, float>;
    static event_t finishedAllAnimations;

    explicit MovementSystem(engine::Game &game);
    void Animate(engine::Game &game, float deltatime);
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
    void setMoving(Transform& root, glm::vec3 goalPosition, int amount, State initialState);
    void setMoved(Transform& rootActorTransform_C, Transform& rootTargetTransform_C, int iAmount, State initialActorState);
    void SetResetting(Transform& unitTransform, State initialState) const;
    void SetAiming(Transform& rootActorTransform_C, Transform& rootTargetTransform_C, int iAmount, State intitialActorState, int iDelay = 0);


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