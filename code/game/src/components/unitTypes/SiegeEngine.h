//
// Created by Julian Kalb on 08/07/2025.
//

#pragma once
#include "engine/ecs/Component.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;



struct SiegeEngine: Component {
    friend ComponentManager;
    friend Entity;

    int m_iUsedAmount = 0;
    int m_iUsedAmountNew = 0;
    int cost;
protected:
    SiegeEngine(guid_t owner, int cost):
        Component(owner),
        cost(cost){}
};
