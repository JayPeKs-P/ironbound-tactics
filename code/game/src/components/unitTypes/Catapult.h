//
// Created by julia on 24/06/2025.
//

#pragma once
#include "SiegeEngine.h"
// #include "Unit.h"
#include "engine/ecs/Component.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

struct Catapult: Component, SiegeEngine{
    friend ComponentManager;
    friend Entity;
private:
    Catapult(guid_t owner, int amount)
    : Component(owner),
    SiegeEngine(400, 20, 150, 80, 12, amount, 5, 3){}
};