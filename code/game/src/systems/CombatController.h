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
    static event_t turnStart;
    static event_t turnEnd;
    static event_t playerDead;
    static event_t enemyDead;

    using eventAction_t = engine::events::Event<CombatController, guid_t, guid_t, int >;
    static eventAction_t onBeforeAttack;
    static eventAction_t onAttack;
    static eventAction_t onAfterAttack;
    static eventAction_t onUse;

    CombatController(engine::Game &game );

    void handleTurn();
private:
    void init(engine::Game &game, int amountInf, int amountArc, int amountCat);
    void setEnemy(engine::Game &game);
    void chooseAttackTarget(guid_t attacker, const UnitCategory &target, const int &amount);
    void runEnemyTurn();
    void scheduleAttack(guid_t attacker, guid_t target, int amount);

    bool justDied = false;
    bool newTurn = false;
    bool endOfTurn = false;
    int turnCount = 1;

    guid_t pInf_E;
    guid_t pArc_E;
    guid_t pCat_E;

    guid_t eInf_E;
    guid_t eArc_E;
    guid_t eCat_E;

    std::shared_ptr<engine::Game::event_t::handle_t> handle = std::make_shared<engine::Game::event_t::handle_t>();
};

}