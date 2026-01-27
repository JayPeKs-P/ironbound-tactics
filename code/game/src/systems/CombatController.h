//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/System.h"
#include "engine/Events.h"


#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"

namespace gl3 {
enum class CombatState {
    IDLE,
    RESET_ENEMY,
    INITIALIZING,
    STARTING_NEW_ROUND,
    BEGIN_TURN,
    ENEMY_TURN,
    MAIN_PHASE,
    ANIMATION,
    DAMAGE_STEP,
    EVALUATE_END,
    VICTORY,
    DEFEAT,
    PREPARE,
    WAIT_NEXT_ROUND
};

class CombatController: public engine::ecs::System
{
public:
    using event_t = engine::events::Event<CombatController>;
    static  event_t initialize;
    static event_t turnStart;
    static event_t onBeforeDamageStep;
    static event_t turnEnd;
    static event_t playerDead;
    static event_t enemyDead;

    using eventAction_t = engine::events::Event<CombatController, guid_t, guid_t, int >;
    static eventAction_t onBeforeAttack;
    static eventAction_t onAttack;
    static eventAction_t onAfterAttack;
    static eventAction_t onUse;

    static int turnCount;
    static int roundCount;

    CombatController(engine::Game &game );
    [[nodiscard]] static CombatState getCombatState()
    {
        return currentState;
    }
    static void setState(CombatState newState)
    {
        currentState = newState;
    }

    void handleTurn();
private:
    void init(engine::Game &game, int amountInf, int amountArc, int amountCat);
    void setEnemy(engine::Game &game);
    void chooseAttackTarget(guid_t iAttacker, const UnitCategory &selectionTarget, const int &iAmount);
    void runEnemyTurn();
    void scheduleAttack(guid_t iActor, guid_t iTarget, int iAmountActors);
    void HelperScheduleUse(guid_t iActor, guid_t iTarget, guid_t iTargetInstanceAmount) const;


    guid_t iInfantryPlayer;
    guid_t iArcherPlayer;
    guid_t iCatapultPlayer;

    guid_t iInfantryEnemy;
    guid_t iArcherEnemy;
    guid_t iCatapultEnemy;

    static CombatState currentState;

};

}