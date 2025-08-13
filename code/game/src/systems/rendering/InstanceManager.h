//
// Created by Julian Kalb on 18/07/2025.
//

#pragma once
#include "engine/ecs/System.h"
#include "engine/sceneGraph/Transform.h"

namespace gl3 {

class InstanceManager: public engine::ecs::System{
public:
    explicit InstanceManager(engine::Game &game);
    void init(engine::Game &game);
    void terminate(engine::Game &game);
    void update(engine::Game &game, engine::sceneGraph::Transform* root);

};

} // gl3