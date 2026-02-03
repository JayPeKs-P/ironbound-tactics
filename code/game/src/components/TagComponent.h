//
// Created by Julian Kalb on 04/07/2025.
//

#pragma once
#include "engine/ecs/Component.h"

namespace gl3 {
using engine::ecs::Component;
using engine::ecs::ComponentManager;
using engine::ecs::guid_t;
using engine::ecs::Entity;

    // Do not extend this class. Logic in multiple cases depend on existence of exactly two valid values.
enum class Tag {PLAYER, ENEMY};

class TagComponent: public Component {
    friend ComponentManager;
    friend Entity;
public:
    Tag value;

private:
    explicit TagComponent(guid_t owner, Tag tag) : Component(owner), value(tag) {}



};

} // gl3