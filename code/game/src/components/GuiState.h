//
// Created by Julian Kalb on 06/07/2025.
//

#pragma once
#include "engine/ecs/Component.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

enum class GuiScene
{
    MAIN_MENU,
    UNIT_SELECTION,
    COMBAT_MENU
};

struct GuiState: Component {
    friend ComponentManager;
    friend Entity;

    GuiScene current;

private:
    explicit GuiState (guid_t owner, GuiScene scene) : Component(owner), current(scene) {}
};