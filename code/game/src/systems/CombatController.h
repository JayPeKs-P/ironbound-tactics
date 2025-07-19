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
    //TODO extract those functions into seperate CombatHelper file(s), maybe even implement them as templatefunctions
    void chooseAttackTarget(Unit* unit, Category selection, int amount);
    void chooseAttackTarget(SiegeEngine *siege, Category selection, int amount);
    // void setAmount(Unit* unit,  int value);
    // void setAmount(SiegeEngine *siege, int value);
    // float attack(Unit* unit, int amount);
    // float attack(SiegeEngine *siege, int amount);
    // void takeDamage(Unit* unit, float damage);
    // void takeDamage(SiegeEngine*, float damage);
    // void use(int amount, Unit* unit, SiegeEngine* siege);
    // void reset(Unit* unit, int amount);
    // void reset(SiegeEngine* siege, int amount);
    // std::mt19937 rng{std::random_device{}()};
    // std::uniform_int_distribution<int> dist{0, 99};
    Infantry* pInf_C = nullptr;
    Archer* pArc_C = nullptr;
    Catapult* pCat_C = nullptr;

    Infantry* eInf_C = nullptr;
    Archer* eArc_C = nullptr;
    Catapult* eCat_C = nullptr;

};

}