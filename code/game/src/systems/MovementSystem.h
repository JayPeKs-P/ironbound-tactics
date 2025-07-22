//
// Created by julia on 22/07/2025.
//
#pragma once
#include "engine/ecs/System.h"

namespace gl3 {

class MovementSystem: public engine::ecs::System{
public:
    explicit MovementSystem(engine::Game &game);
    void moveTo(engine::Game &game, glm::vec3 goalPosition);
    void update(engine::Game &game, engine::sceneGraph::Transform* root);
private:
    glm::vec3 movementVector;
};

} // gl3