//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/System.h"
#include "engine/Events.h"
#include <random>

#include "../components/CombatSelection.h"
#include "../entities/unitTypes/Archer.h"
#include "../entities/unitTypes/Infantry.h"
#include "../entities/objectTypes/Catapult.h"

#include "../entities/Unit.h"

namespace gl3 {

class CombatController: public engine::ecs::System {
public:
    // TODO: check if this typedef needs to add <..., CombatController&> like in wp3 engine/game.h
    using eventCC_t = engine::events::Event<CombatController>;
    CombatController(engine::Game &game );
    ~CombatController();

    // events for combat and turn structure
    eventCC_t onStartCombat;
    eventCC_t onBeforeTurn;
    eventCC_t onAfterTurn;
    eventCC_t onEndCombat;

    void handleTurn(bool newRound);

    void init(engine::Game &game);

private:
    float CombatController::attack(Unit* unit);
    void CombatController::takeDamage(float damage);
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