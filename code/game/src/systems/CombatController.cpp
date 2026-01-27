//
// Created by Julian Kalb on 25/02/2025.
//
#include <iostream>
#include <ostream>
#include "json.hpp"

#include "engine/Game.h"
#include "CombatController.h"

#include "../logic/LibCombatFunctions.h"
#include "../logic/EnemyAI.h"



#include "MovementSystem.h"
using json = nlohmann::json;

#include "../gui/GuiCombat.h"
#include "../gui/GuiUnitSelection.h"
#include "../components/TagComponent.h"
#include "engine/logic/ActionRegistry.h"

#include "../components/unitTypes/UnitCategory.h"
#include "engine/util/Debug.h"


using namespace gl3;

CombatController::event_t CombatController::initialize;
CombatController::event_t CombatController::turnStart;
CombatController::event_t CombatController::turnEnd;
CombatController::event_t CombatController::onBeforeDamageStep;
CombatController::event_t CombatController::playerDead;
CombatController::event_t CombatController::enemyDead;


CombatController::eventAction_t CombatController::onBeforeAttack;
CombatController::eventAction_t CombatController::onAttack;
CombatController::eventAction_t CombatController::onAfterAttack;
CombatController::eventAction_t CombatController::onUse;

int CombatController::turnCount = 0;
int CombatController::roundCount = 1;

CombatState CombatController::currentState = CombatState::IDLE;
bool done = false;
void CombatController::handleTurn()
{
    switch (currentState) {
    case CombatState::IDLE:     // Base state when inactive
        break;
    case CombatState::RESET_ENEMY:
        engine.componentManager.removeComponent<Unit>(iInfantryEnemy);
        engine.componentManager.removeComponent<Unit>(iArcherEnemy);
        engine.componentManager.removeComponent<Unit>(iCatapultEnemy);
        engine.componentManager.getComponent<SiegeEngine>(iCatapultEnemy).useableAmount = 0;
        setState(CombatState::STARTING_NEW_ROUND);
        break;
    case CombatState::STARTING_NEW_ROUND:
        if (!done) setEnemy(engine);
        done = true;
        break;
    case CombatState::INITIALIZING:     // Called once after starting a new round of combat
        {
        done = false;
        initialize.invoke();
        auto pEnemyAI = EnemyAI::GetInstance();
        pEnemyAI->setGuids(engine);
        setState(CombatState::BEGIN_TURN);
        break;}
    case CombatState::BEGIN_TURN:
        {
        turnCount++;
        turnStart.invoke();
        setState(CombatState::ENEMY_TURN);
        break;}
    case CombatState::ENEMY_TURN:
        runEnemyTurn();
        currentState = CombatState::MAIN_PHASE;
        break;
    case CombatState::ANIMATION:
        onBeforeDamageStep.invoke();
        break;
    case CombatState::MAIN_PHASE:
        // Warten auf startRound
        break;
    case CombatState::DAMAGE_STEP:
        if (engine.actionRegister.advance()) {
            int unused = engine::ecs::invalidID;
            onAfterAttack.invoke(unused, unused, unused);
            currentState = CombatState::EVALUATE_END;
        }
        break;

    case CombatState::EVALUATE_END:
    {
        if (!engine.entityManager.checkIfEntityHasComponent<Unit>(iInfantryPlayer, iArcherPlayer, iCatapultPlayer, iInfantryEnemy, iArcherEnemy, iCatapultEnemy)) return;

        auto pInfU_C = engine.componentManager.getComponent<Unit>(iInfantryPlayer);
        auto pArcU_C = engine.componentManager.getComponent<Unit>(iArcherPlayer);
        auto pCatU_C = engine.componentManager.getComponent<Unit>(iCatapultPlayer);

        auto eInfU_C = engine.componentManager.getComponent<Unit>(iInfantryEnemy);
        auto eArcU_C = engine.componentManager.getComponent<Unit>(iArcherEnemy);
        auto eCatU_C = engine.componentManager.getComponent<Unit>(iCatapultEnemy);

        bool playerDeadNow = (pInfU_C.totalAmount <= 0 && pArcU_C.totalAmount <= 0 && pCatU_C.totalAmount <= 0);
        bool enemyDeadNow  = (eInfU_C.totalAmount <= 0 && eArcU_C.totalAmount <= 0 && eCatU_C.totalAmount <= 0);

        if (playerDeadNow) {
            currentState = CombatState::DEFEAT;
            playerDead.invoke();
        } else if (enemyDeadNow) {
            turnEnd.InvokeAndClear();
            currentState = CombatState::VICTORY;
            // enemyDead.invoke();   //TODO: change this back
        } else {
            turnEnd.InvokeAndClear();
            currentState = CombatState::BEGIN_TURN;
        }
        break;
    }

    case CombatState::VICTORY:
        enemyDead.invoke();   //TODO: remove this again and uncomment above
        setState(CombatState::REWARD_PHASE);
        break;
    case CombatState::REWARD_PHASE:
        break;
    case CombatState::DEFEAT:
        break;
    case CombatState::PREPARE:
        setEnemy(engine);
        setState(CombatState::WAIT_NEXT_ROUND);
        break;
    case CombatState::WAIT_NEXT_ROUND:
        turnCount = 0;
        setState(CombatState::RESET_ENEMY);
        roundCount++;
        break;
    }
}

CombatController::CombatController(engine::Game &game):
    System(game)
{
    GuiUnitSelection::onAccept.addListener([&](int amountInf, int amountArc, int amountCat)
    {
        init(game, amountInf, amountArc, amountCat);
        setState(CombatState::RESET_ENEMY);
    });
    MovementSystem::finishedAllAnimations.addListener([&](bool finished)
    {
        setState(CombatState::DAMAGE_STEP);
    });
    turnStart.addListener([=]()
    {
    });
    playerDead.addListener([&]()
    {
    });

////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_LOG
    turnStart.addListener([=]()
    {
DEBUG_LOG(
    << "=======| This is turn: "
    << turnCount
    << " |======="
    );
    });

    turnEnd.addListener([&]()
    {
        // engine.componentManager.forEachComponent<Unit>([&](Unit& unit_C)
        // {
        //     if (engine.componentManager.hasComponent<SiegeEngine>(unit_C.entity())) {
        //
        //     }
        //     else {
        //         unit_C.availableAmount = (unit_C.totalAmount < 0) ? 0 : unit_C.totalAmount;
        //     }
        // });
    DEBUG_LOG(
        << "=======| End of turn: "
        << turnCount
        << " |======="
        );
    });
    onBeforeAttack.addListener([&](guid_t attacker, guid_t target, int amount)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onBeforeAttack'"
            );
    });
    onAttack.addListener([&](guid_t attacker, guid_t target, int amount)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onAttack'"
            );
    });
    onAfterAttack.addListener([&](guid_t attacker, guid_t target, int amount)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onAfterAttack'"
            );
    });
    onUse.addListener([&](guid_t actor , guid_t object, int amount)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onUse'"
            );
    });
#endif
}
////////////////////////////////////////////////////////////////////////


    for (auto& [owner, _] : engine.componentManager.getContainer<CombatSelection<GuiCombat>>())
    {
        if (game.componentManager.hasComponent<CombatSelection<GuiCombat>>(owner))
        {
            auto& selectionEvent = engine.componentManager.getComponent<CombatSelection<GuiCombat>>(owner);
            selectionEvent.attack.addListener([&](UnitCategory selectionPlayer, int iAmount, UnitCategory selectionEnemy)
            {
                if (selectionPlayer == UnitCategory::INFANTRY)
                {
                    chooseAttackTarget(iInfantryPlayer, selectionEnemy, iAmount);
                }
                else if (selectionPlayer == UnitCategory::ARCHER)
                {
                    chooseAttackTarget(iArcherPlayer, selectionEnemy, iAmount);
                }
                else if (selectionPlayer == UnitCategory::CATAPULT)
                {
                    chooseAttackTarget(iCatapultPlayer, selectionEnemy, iAmount);
                }
            });

            selectionEvent.use.addListener([&](UnitCategory selectionUnit, int iTargetInstanceAmount, UnitCategory selectionSiegeEngine)
            {
                if (selectionUnit == UnitCategory::INFANTRY)
                {
                    if (selectionSiegeEngine == UnitCategory::CATAPULT)
                    {
                        HelperScheduleUse(iInfantryPlayer, iCatapultPlayer, iTargetInstanceAmount);
                    }
                }
                else if (selectionUnit == UnitCategory::ARCHER)
                {
                    if (selectionSiegeEngine == UnitCategory::CATAPULT)
                    {
                        HelperScheduleUse(iArcherPlayer, iCatapultPlayer, iTargetInstanceAmount);
                    }
                }
            });
        }
    }
}

void CombatController::init(engine::Game &game, int amountInf, int amountArc, int amountCat)
{
    game.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {

        auto &tag = game.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag == Tag::PLAYER)
        {
            auto pLibCombat = LibCombatFunctions::GetInstance(engine);
            if (unit.category == UnitCategory::INFANTRY)
            {
                iInfantryPlayer = unit.entity();
                pLibCombat->setAmount(iInfantryPlayer, amountInf);
            }else if (unit.category == UnitCategory::ARCHER)
            {
                iArcherPlayer = unit.entity();
                pLibCombat->setAmount(iArcherPlayer, amountArc);
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                iCatapultPlayer = unit.entity();
                auto pCatSE_C = &game.componentManager.getComponent<SiegeEngine>(iCatapultPlayer);
                pLibCombat->setAmount(iCatapultPlayer, amountCat / pCatSE_C->cost);
            }
        }else if (tag == Tag::ENEMY)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                iInfantryEnemy = unit.entity();
                // engine.componentManager.removeComponent<Unit>(eInf_E);
            }else if (unit.category == UnitCategory::ARCHER)
            {
                iArcherEnemy = unit.entity();
                // engine.componentManager.removeComponent<Unit>(eArc_E);
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                iCatapultEnemy = unit.entity();
                // engine.componentManager.removeComponent<Unit>(eCat_E);
            }
        }
    });
}

void CombatController::setEnemy(engine::Game& game)
{
    static json ArmySetup;

    if (ArmySetup.empty()) {
        std::ifstream f("assets/presets/EnemyArmies.json");
        if (!f.is_open()) throw std::runtime_error("Could not open EnemyArmies.json");
        f >> ArmySetup;
    }

    std::string difficulty;
    if (roundCount >= 10) difficulty = "HARD";
    else if (roundCount >= 5) difficulty = "MEDIUM";
    else difficulty = "EASY";

    static std::random_device random;
    static std::mt19937 generate(random());
    std::uniform_int_distribution<> dist(1, 5);
    std::string key = std::to_string(dist(generate));

    if (!ArmySetup.contains(difficulty) || !ArmySetup[difficulty].contains(key))
    {
        throw std::runtime_error("CombatController::setEnemy() -> Missing enemy for: " + difficulty + " -> " + key);
    }
    json enemySetup = ArmySetup[difficulty][key];

    auto pLibCombat = LibCombatFunctions::GetInstance(engine);
    auto eInfU_C = &engine.componentManager.addComponent<Unit>(iInfantryEnemy, enemySetup.at("Infantry"));
    auto eArcU_C = &engine.componentManager.addComponent<Unit>(iArcherEnemy, enemySetup.at("Archer"));
    auto eCatU_C = &engine.componentManager.addComponent<Unit>(iCatapultEnemy, enemySetup.at("Catapult"));

    pLibCombat->setAmount(iInfantryEnemy, enemySetup.at("amountInf"));
    pLibCombat->setAmount(iArcherEnemy, enemySetup.at("amountArc"));
    pLibCombat->setAmount(iCatapultEnemy, enemySetup.at("amountCat"));
}

void CombatController::chooseAttackTarget(guid_t iAttacker, const UnitCategory &selectionTarget, const int &iAmount)
{
    auto attackerUnit_C = &engine.componentManager.getComponent<Unit>(iAttacker);
    switch (selectionTarget)
    {
        case UnitCategory::INFANTRY:
            scheduleAttack(iAttacker, iInfantryEnemy, iAmount);
            break;
        case UnitCategory::ARCHER:
            scheduleAttack(iAttacker, iArcherEnemy, iAmount);
            break;
        case UnitCategory::CATAPULT:
            scheduleAttack(iAttacker, iCatapultEnemy, iAmount);
            break;
    }
    attackerUnit_C->availableAmount -= iAmount;
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << "Player schedules attack: "
        << unitCategory_to_string(attackerUnit_C->category)
        <<" targets "
        << unitCategory_to_string(selectionTarget)
        << " with "
        << iAmount
        );
#endif
}

void CombatController::runEnemyTurn()
{
    auto pEnemyAI = EnemyAI::GetInstance();
    auto options = pEnemyAI->generateOptions(engine);
    for (auto& option: options)
    {
        if (!engine.entityManager.checkIfEntityHasComponent<Unit>(option.iActor_ID, option.iTarget_ID))
        {
            throw("CombatController::runEnemyTurn missing unit_C");
        }
        auto pActorUnit_C = &engine.componentManager.getComponent<Unit>(option.iActor_ID);
        auto pTargetUnit_C = &engine.componentManager.getComponent<Unit>(option.iTarget_ID);
        auto pTargetTag_C = &engine.componentManager.getComponent<TagComponent>(option.iTarget_ID);

        if (pActorUnit_C->availableAmount >= option.iTargetAmount)
        {
            if (engine.componentManager.hasComponent<SiegeEngine>(option.iTarget_ID) && pTargetTag_C->value == Tag::ENEMY)
            {
                auto pTargetSiegeEngine_C = &engine.componentManager.getComponent<SiegeEngine>(option.iTarget_ID);
                if (pTargetSiegeEngine_C->useableAmount < option.iTargetAmount) continue;
                HelperScheduleUse(option.iActor_ID, option.iTarget_ID, option.iTargetAmount);
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << "AI uses: "
        << option.iTargetAmount
        << " of "
        << unitCategory_to_string(pActorUnit_C->category)
        << " for "
        << option.iTargetAmount / targetSE_C->cost
        << " of "
        << unitCategory_to_string(pTargetUnit_C->category)
        );
#endif
            }
            else
            {
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << "AI schedules attack: "
        << unitCategory_to_string(pActorUnit_C->category)
        << " targets "
        << unitCategory_to_string(pTargetUnit_C->category)
        << " with "
        << option.iTargetAmount
        );
#endif

                scheduleAttack(option.iActor_ID, option.iTarget_ID, option.iTargetAmount);
                pActorUnit_C->availableAmount -= option.iTargetAmount;
            }
        }
    }
}

void CombatController::scheduleAttack(guid_t iActor, guid_t iTarget, int iAmountActors)
{
    if (!engine.entityManager.checkIfEntityHasComponent<Unit>(iActor, iTarget)) throw("CombatController::scheduleAttack Missing unit_C");
    auto pActorUnit_C = &engine.componentManager.getComponent<Unit>(iActor);
    auto pTargetUnit_C = &engine.componentManager.getComponent<Unit>(iTarget);

    onBeforeAttack.invoke(iActor, iTarget, iAmountActors);
    // Hardcoded for end of turn movement of infantry. need to change that
    engine.actionRegister.scheduleAction(pActorUnit_C->speed-1, [=]()
    {
        onAttack.invoke(iActor, iTarget, iAmountActors);
    });
    engine.actionRegister.scheduleAction(pActorUnit_C->speed,[=] ()
    {
        auto pLibCombat = LibCombatFunctions::GetInstance(engine);
        onAttack.invoke(iActor, iTarget, iAmountActors);
        pLibCombat->takeDamage(iTarget, pLibCombat->attack(iActor, iAmountActors));

        turnEnd.addListener([=](){
            auto pLibCombatCallback = LibCombatFunctions::GetInstance(engine);
            pLibCombatCallback->ResetUnit(iActor, iAmountActors);
        });

#ifdef DEBUG_MODE
    DEBUG_LOG(
        << unitCategory_to_string(pActorUnit_C->category)
        <<" attacked "
        << unitCategory_to_string(pTargetUnit_C->category)
        << " with "
        << iAmountActors
        );
#endif
    });
}

void CombatController::HelperScheduleUse(guid_t iActor, guid_t iTarget, guid_t iTargetInstanceAmount) const {
    if (!engine.entityManager.checkIfEntityHasComponent<Unit>(iActor, iTarget)) throw("CombatController::use() Missing unit_C");
    if (!engine.entityManager.checkIfEntityHasComponent<SiegeEngine>(iTarget)) throw("CombatController::unit() Missing siege_C");

    auto pActorUnit_C = &engine.componentManager.getComponent<Unit>(iActor);
    // auto pTargetUnit_C = &engine.componentManager.getComponent<Unit>(iTarget);
    auto pTargetSiegeEngine_C = &engine.componentManager.getComponent<SiegeEngine>(iTarget);
    const int iActorCost = iTargetInstanceAmount * pTargetSiegeEngine_C->cost;

    onBeforeAttack.invoke(iActor, iTarget, iActorCost);
    engine.actionRegister.scheduleAction(MIN_SPEED_VALUE,[=] ()
    {
        auto pLibCombat = LibCombatFunctions::GetInstance(engine);
        pLibCombat->UseSiegeEngine(iTargetInstanceAmount, iActor, iTarget);
        onUse.invoke(iActor, iTarget, iTargetInstanceAmount);

        turnEnd.addListener([=](){
            auto pLibCombatCallback = LibCombatFunctions::GetInstance(engine);
            pLibCombatCallback->ResetUnit(iTarget, iTargetInstanceAmount);
            pLibCombatCallback->ResetUnit(iActor, iActorCost);
        });
    });

    pActorUnit_C->availableAmount -= iActorCost;
}

