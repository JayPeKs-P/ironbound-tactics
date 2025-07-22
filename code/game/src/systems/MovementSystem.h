//
// Created by julia on 22/07/2025.
//
#pragma once
#include "CombatController.h"
#include "CombatController.h"
#include "CombatController.h"
#include "CombatController.h"
#include "CombatController.h"
#include "CombatController.h"
#include "engine/ecs/System.h"

using gl3::engine::sceneGraph::Transform;
namespace gl3 {

class MovementSystem: public engine::ecs::System{
public:
    explicit MovementSystem(engine::Game &game);
    void moveTo(engine::Game &game, glm::vec3 goalPosition, float deltatime);
    void update(engine::Game &game, Transform* root);
    bool moveStraight(Transform& root, glm::vec3 direction, float deltatime);
    bool moveCurved(Transform& root, glm::vec3 goal, float compression, float deltatime);
private:
    glm::vec3 movementVector;
};

} // gl3