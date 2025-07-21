//
// Created by Julian Kalb on 07/07/2025.
//

#pragma once

#include "engine/ecs/Component.h"
#include "engine/Events.h"
#include"unitTypes/UnitCategory.h"

struct Unit;
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;
//TODO create GuiSelection Parent Interface, that stores events of any kind
template <typename G>
struct CombatSelection : Component {
    friend ComponentManager;
    friend Entity;

    std::shared_ptr<Unit> selectionOne;
    std::shared_ptr<Unit> selectionTwo;

    using event_t = gl3::engine::events::Event<G, UnitCategory, int, UnitCategory>;
    event_t attack;
    event_t defend;
    event_t use;

private:
    explicit CombatSelection(guid_t owner) : Component(owner) {}
};
