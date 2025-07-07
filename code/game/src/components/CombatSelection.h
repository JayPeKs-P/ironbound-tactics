//
// Created by Julian Kalb on 07/07/2025.
//

#pragma once

#include "engine/ecs/Component.h"
#include "engine/Events.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

enum class Category {INFANTRY, ARCHER, SIEGE, CATAPULT, ASSAULT_COVER};
template <typename G>
struct CombatSelection : Component {
    friend ComponentManager;
    friend Entity;

    using event_t = gl3::engine::events::Event<G,
    Category,
    int,
    Category>;
    event_t attack;
    event_t defend;
    event_t use;

private:
    explicit CombatSelection(guid_t owner) : Component(owner) {}
};
