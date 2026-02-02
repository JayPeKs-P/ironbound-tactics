//
// Created by Julian Kalb on 01/02/2026.
//
#pragma once
#include "engine/ecs/Component.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

struct Icon : Component {
    friend ComponentManager;
    friend Entity;

private:
    Icon(guid_t owner) : Component(owner) {
    }
};
struct Bounce : Component {
    friend ComponentManager;
    friend Entity;

    float m_fCurrentTime = 0.0f;
    float m_fSpeed = 10.0f;
    float m_fAmplitude = 0.02f;
    float m_fBase = 0.0f;

private:
    Bounce(guid_t owner) : Component(owner) {}
};
