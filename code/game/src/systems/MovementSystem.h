//
// Created by julia on 22/07/2025.
//
#pragma once
#include "engine/sceneGraph/Transform.h"
#include "engine/ecs/System.h"
#include "engine/Events.h"


enum class State;
using gl3::engine::sceneGraph::Transform;
namespace gl3 {

class MovementSystem: public engine::ecs::System{
public:
    using event_t = gl3::engine::events::Event<MovementSystem, float>;
    static event_t finishAnimation;
    static event_t finishedAllAnimations;

    explicit MovementSystem(engine::Game &game);
    void moveTo(engine::Game &game, float deltatime);
    void update(engine::Game &game, Transform* root);
    void moveStraight(Transform& transform, glm::vec3 direction, float deltatime, State endState, float delay);
    void moveCurved(Transform& root, glm::vec3 goal, float compression, float deltatime);
private:
    void setMoving(Transform& root, glm::vec3 goalPosition, int amount, State initialState);
    void setMoved(Transform& root, glm::vec3 goalPosition, int amount, State initialState);
    void setAttacking(Transform& root, glm::vec3 targetPosition, int amount);
    glm::vec3 movementVector;
    glm::vec3 playerPendingPosition = glm::vec3(-0.2, -0.3, 0.0);
    glm::vec3 enemyPendingPosition = glm::vec3(0.0, 0.3, 0.0);
    bool m_bAllAnimationsFinished = false;

    float countdown = 0.5f;
};

} // gl3