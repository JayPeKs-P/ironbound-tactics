//
// Created by Julian Kalb on 18/07/2025.
//
#pragma once
#include <engine/ecs/Component.h>
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

enum class State {IDLE, MOVING, ATTACKING, RESETTING, PREPARING};
struct UnitState: Component
{
    friend ComponentManager;
    friend Entity;
    public:
    State state = State::IDLE;
    float movementSpeed = 0.2;
    float traveledDistance = 0;

private:
    explicit UnitState(guid_t owner):
    Component(owner) {}
};