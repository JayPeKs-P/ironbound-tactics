//
// Created by Julian Kalb on 18/07/2025.
//
#pragma once
#include <engine/ecs/Component.h>
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

struct UnitState: Component
{
    friend ComponentManager;
    friend Entity;
private:
    explicit UnitState(guid_t ownder):
    Component(ownder) {}
};