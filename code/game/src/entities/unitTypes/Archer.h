//
// Created by Julian Kalb on 13/06/2025.
//
#pragma once
#include "../Unit.h"
#include "engine/ecs/Component.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

struct Archer: Unit, Component{
    friend ComponentManager;
    friend Entity;
private:
    Archer(guid_t owner, int amount)
    : Component(owner),
    Unit(80, 20, 40, 85, 8, amount){}
};
