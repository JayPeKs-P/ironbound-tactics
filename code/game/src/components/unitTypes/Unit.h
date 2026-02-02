//
// Created by Julian Kalb on 12/12/2024.
//

#pragma once
#include <fstream>
#include "engine/ecs/Component.h"
#include "UnitCategory.h"

#define MIN_SPEED_VALUE 1

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

struct Unit: Component {
    friend ComponentManager;
    friend Entity;

    float hpValue;
    float fLeftoverDamage = 0.0f;
    float armorValue;
    float attackValue;
    int accuracy;
    int critChance;
    float critMultiplier = 2.0f;
    int lifetimeMaxAmount;
    int totalAmount;
    int totalAmountLastFrame;
    int m_iScheduledForUse = 0;
    int availableAmount;
    int speed;
    UnitCategory category;
    std::string texturePath;
    bool m_bShouldHaveIcon = true;
protected:
    Unit (guid_t owner, const std::string& preset);

};
