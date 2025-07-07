//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/System.h"
#include <random>

#include "../components/unitTypes/Archer.h"
#include "../components//unitTypes/Infantry.h"
#include "../components/unitTypes/Catapult.h"

#include "../components/unitTypes/Unit.h"

namespace gl3 {

class CombatController: public engine::ecs::System {
public:
    // TODO: check if this typedef needs to add <..., CombatController&> like in wp3 engine/game.h
    CombatController(engine::Game &game );
    ~CombatController();

    void handleTurn(bool newRound);

    void init(engine::Game &game);

private:
    void setAmount(Unit* unit, int amount);
    float CombatController::attack(Unit* unit, int amount);
    void CombatController::takeDamage(Unit* unit, float damage);
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, 99};
    Infantry* pInf_C = nullptr;
    Archer* pArc_C = nullptr;
    Catapult* pCat_C = nullptr;

    Infantry* eInf_C = nullptr;
    Archer* eArc_C = nullptr;
    Catapult* eCat_C = nullptr;

};

}