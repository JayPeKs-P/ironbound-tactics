//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include <random>
#include "engine/ecs/System.h"
#include "engine/Events.h"

#include "../components/CombatSelection.h"
#include "../components/unitTypes/Archer.h"
#include "../components//unitTypes/Infantry.h"
#include "../components/unitTypes/Catapult.h"

#include "../components/unitTypes/Unit.h"

namespace gl3 {

class CombatController: public engine::ecs::System
{
public:
    // TODO: check if this typedef needs to add <..., CombatController&> like in wp3 engine/game.h
    CombatController(engine::Game &game );
    ~CombatController();

    void handleTurn();
    void init(engine::Game &game);
private:
    void chooseAttackTarget(Unit* unit, Category selection, int amount);
    void chooseAttackTarget(SiegeEngine *siege, Category selection, int amount);
    Infantry* pInf_C = nullptr;
    Archer* pArc_C = nullptr;
    Catapult* pCat_C = nullptr;

    Infantry* eInf_C = nullptr;
    Archer* eArc_C = nullptr;
    Catapult* eCat_C = nullptr;

};

}