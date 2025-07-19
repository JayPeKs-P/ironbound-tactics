//
// Created by Julian Kalb on 12/12/2024.
//

#pragma once
#include "engine/ecs/Component.h"
#include "UnitCategory.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

struct Unit: Component {
    friend ComponentManager;
    friend Entity;

    float hpValue;
    float armorValue;
    float attackValue;
    int accuracy;
    int critChance;
    float critMultiplier = 2.0f;
    int lifetimeMaxAmount;
    int totalAmount;
    int totalAmountLastFrame;
    int availableAmount;
    int speed;
    UnitCategory category;
protected:
    Unit (guid_t owner, UnitCategory category, float hp, float def, float atk, int acc, int critc, int amount, int speed):
        Component(owner),
        category(category),
        hpValue(hp),
        armorValue(def),
        attackValue(atk),
        accuracy(acc),
        critChance(critc),
        totalAmount(amount),
        totalAmountLastFrame(amount),
        availableAmount(amount),
        lifetimeMaxAmount(amount),
        speed(speed) { };

};
