//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/System.h"
#include "engine/Events.h"


#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"

namespace gl3 {

class CombatController: public engine::ecs::System
{
public:
    using event_t = engine::events::Event<CombatController>;
    event_t turnStart;
    event_t turnEnd;

    using eventAttack_t = engine::events::Event<CombatController, Unit*, Unit*, int >;
    static eventAttack_t onBeforeAttack;
    static eventAttack_t onAttack;
    static eventAttack_t onAfterAttack;
    static engine::events::Event<CombatController, int, Unit*, SiegeEngine*> onUse;

    CombatController(engine::Game &game );

    void handleTurn();
    void init(engine::Game &game, int amountInf, int amountArc, int amountCat);
private:
    void chooseAttackTarget(Unit* attacker, const UnitCategory &target, const int &amount);
    void runEnemyTurn();
    void scheduleAttack(Unit* attacker, Unit* target, int amount);

    bool endOfTurn = true;
    int turnCount = 1;

    Unit* pInfU_C = nullptr;
    Unit* pArcU_C = nullptr;
    Unit* pCatU_C = nullptr;
    SiegeEngine* pCatSE_C = nullptr;

    Unit* eInfU_C = nullptr;
    Unit* eArcU_C = nullptr;
    Unit* eCatU_C = nullptr;
    SiegeEngine* eCatSE_C = nullptr;

};

}