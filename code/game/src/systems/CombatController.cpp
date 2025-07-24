//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"
#include "../logic/CombatFunctions.h"
#include "../logic/ActionEvaluation.h"

#include <iostream>
#include <ostream>
#include <json.hpp>
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
        engine.componentManager.removeComponent<Unit>(eInf_E);
        engine.componentManager.removeComponent<Unit>(eArc_E);
        engine.componentManager.removeComponent<Unit>(eCat_E);
        engine.componentManager.getComponent<SiegeEngine>(eCat_E).useableAmount = 0;
        setState(CombatState::STARTING_NEW_ROUND);
        break;
    case CombatState::STARTING_NEW_ROUND:
        if (!done) setEnemy(engine);
        done = true;
        break;
    case CombatState::INITIALIZING:     // Called once after starting a new round of combat
        done = false;
        initialize.invoke();
        ActionEvaluation::setGuids(engine);
        setState(CombatState::BEGIN_TURN);
        break;
    case CombatState::BEGIN_TURN:
        turnCount++;
        turnStart.invoke();
        setState(CombatState::ENEMY_TURN);
        break;
    case CombatState::ENEMY_TURN:
        runEnemyTurn();
        currentState = CombatState::MAIN_PHASE;
        break;
    case CombatState::MAIN_PHASE:
        // Warten auf startRound
        break;
    case CombatState::DAMAGE_STEP:
        if (engine.actionRegister.advance()) {
            currentState = CombatState::EVALUATE_END;
        }
        break;

    case CombatState::EVALUATE_END:
    {
        if (!checkIfEntityHasComponent<Unit>(pInf_E, pArc_E, pCat_E, eInf_E, eArc_E, eCat_E)) return;

        auto pInfU_C = engine.componentManager.getComponent<Unit>(pInf_E);
        auto pArcU_C = engine.componentManager.getComponent<Unit>(pArc_E);
        auto pCatU_C = engine.componentManager.getComponent<Unit>(pCat_E);

        auto eInfU_C = engine.componentManager.getComponent<Unit>(eInf_E);
        auto eArcU_C = engine.componentManager.getComponent<Unit>(eArc_E);
        auto eCatU_C = engine.componentManager.getComponent<Unit>(eCat_E);

        bool playerDeadNow = (pInfU_C.totalAmount <= 0 && pArcU_C.totalAmount <= 0 && pCatU_C.totalAmount <= 0);
        bool enemyDeadNow  = (eInfU_C.totalAmount <= 0 && eArcU_C.totalAmount <= 0 && eCatU_C.totalAmount <= 0);

        if (playerDeadNow) {
            currentState = CombatState::DEFEAT;
            playerDead.invoke();
        } else if (enemyDeadNow) {
            currentState = CombatState::VICTORY;
            enemyDead.invoke();
        } else {
            turnEnd.invoke();
            currentState = CombatState::BEGIN_TURN;
        }
        break;
    }

    case CombatState::VICTORY:
        setState(CombatState::PREPARE);
        break;

    case CombatState::DEFEAT:
        setState(CombatState::IDLE);
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

    turnEnd.addListener([=]()
    {
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
            selectionEvent.attack.addListener([&](UnitCategory selP, int amount, UnitCategory selE)
            {
                if (selP == UnitCategory::INFANTRY)
                {
                    chooseAttackTarget(pInf_E, selE, amount);
                }else if (selP == UnitCategory::ARCHER)
                {
                    chooseAttackTarget(pArc_E, selE, amount);
                }else if (selP == UnitCategory::CATAPULT)
                {
                    chooseAttackTarget(pCat_E, selE, amount);
                }
            });
            selectionEvent.use.addListener([&](UnitCategory selP, int amount, UnitCategory selPTarget)
            {
                if (selP == UnitCategory::INFANTRY)
                {
                    if (selPTarget == UnitCategory::CATAPULT)
                    {
                        if (!checkIfEntityHasComponent<Unit>(pInf_E, pCat_E)) throw("CombatController::use() Missing unit_C");
                        if (!checkIfEntityHasComponent<SiegeEngine>(pCat_E)) throw("CombatController::unit() Missing siege_C");
                        auto pInfU_C = &engine.componentManager.getComponent<Unit>(pInf_E);
                        auto pCatU_C = &engine.componentManager.getComponent<Unit>(pCat_E);
                        auto pCatSE_C = &engine.componentManager.getComponent<SiegeEngine>(pCat_E);
                        CombatFunctions::use(amount, pInfU_C, pCatSE_C);
                        onUse.invoke(pInf_E, pCat_E, amount);

                        std::shared_ptr<event_t::handle_t> handle = std::make_shared<event_t::handle_t>();
                        *handle = turnEnd.addListener([=](){
                            CombatFunctions::reset(pCatU_C, amount);
                            turnEnd.removeListener(*handle);
                        });
                    }else if (selPTarget == UnitCategory::ASSAULT_COVER)
                    {

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
            if (unit.category == UnitCategory::INFANTRY)
            {
                pInf_E = unit.entity();
                auto pInfU_C = &engine.componentManager.getComponent<Unit>(pInf_E);
                CombatFunctions::setAmount(pInfU_C, amountInf);
            }else if (unit.category == UnitCategory::ARCHER)
            {
                pArc_E = unit.entity();
                auto pArcU_C = &engine.componentManager.getComponent<Unit>(pArc_E);
                CombatFunctions::setAmount(pArcU_C, amountArc);
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                pCat_E = unit.entity();
                auto pCatU_C = &engine.componentManager.getComponent<Unit>(pCat_E);
                auto pCatSE_C = &game.componentManager.getComponent<SiegeEngine>(pCat_E);
                CombatFunctions::setAmount(pCatU_C, amountCat / pCatSE_C->cost);
            }
        }else if (tag == Tag::ENEMY)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                eInf_E = unit.entity();
                // engine.componentManager.removeComponent<Unit>(eInf_E);
            }else if (unit.category == UnitCategory::ARCHER)
            {
                eArc_E = unit.entity();
                // engine.componentManager.removeComponent<Unit>(eArc_E);
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                eCat_E = unit.entity();
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

    // calculate Difficulty depending on won rounds
    std::string difficulty;
    if (roundCount >= 10) difficulty = "HARD";
    else if (roundCount >= 5) difficulty = "MEDIUM";
    else difficulty = "EASY";

    // get random key for specific army of previously defined difficulty
    static std::random_device random;
    static std::mt19937 generate(random());
    std::uniform_int_distribution<> dist(1, 5);
    std::string key = std::to_string(dist(generate));

    if (!ArmySetup.contains(difficulty) || !ArmySetup[difficulty].contains(key))
    {
        throw std::runtime_error("CombatController::setEnemy() -> Missing enemy for: " + difficulty + " -> " + key);
    }
    json enemySetup = ArmySetup[difficulty][key];


    engine.componentManager.addComponent<Unit>(eInf_E, enemySetup.at("Infantry"));
    engine.componentManager.addComponent<Unit>(eArc_E, enemySetup.at("Archer"));
    engine.componentManager.addComponent<Unit>(eCat_E, enemySetup.at("Catapult"));

                auto eInfU_C = &engine.componentManager.getComponent<Unit>(eInf_E);
                CombatFunctions::setAmount(eInfU_C, enemySetup.at("amountInf"));

                auto eArcU_C = &engine.componentManager.getComponent<Unit>(eArc_E);
                CombatFunctions::setAmount(eArcU_C, enemySetup.at("amountArc"));

                auto eCatU_C = &engine.componentManager.getComponent<Unit>(eCat_E);
                CombatFunctions::setAmount(eCatU_C, enemySetup.at("amountCat"));
}

void CombatController::chooseAttackTarget(guid_t attacker, const UnitCategory &target, const int &amount)
{
    auto attackerUnit_C = &engine.componentManager.getComponent<Unit>(attacker);
    switch (target)
    {
        case UnitCategory::INFANTRY:
            scheduleAttack(attacker, eInf_E, amount);
            break;
        case UnitCategory::ARCHER:
            scheduleAttack(attacker, eArc_E, amount);
            break;
        case UnitCategory::CATAPULT:
            scheduleAttack(attacker, eCat_E, amount);
            break;
    }
    attackerUnit_C->availableAmount -= amount;
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << "Player schedules attack: "
        << unitCategory_to_string(attackerUnit_C->category)
        <<" targets "
        << unitCategory_to_string(target)
        << " with "
        << amount
        );
#endif
}
////////////////////////////////////////////////////////////////////////
}

void CombatController::runEnemyTurn()
{
    auto options = ActionEvaluation::generateOptions(engine);
    for (auto& option: options)
    {
        if (!checkIfEntityHasComponent<Unit>(option.actor, option.target))
        {
            throw("CombatController::runEnemyTurn missing unit_C");
        }
        auto actorU_C = &engine.componentManager.getComponent<Unit>(option.actor);
        auto targetU_C = &engine.componentManager.getComponent<Unit>(option.target);
        if (actorU_C->availableAmount >= option.amount)
        {
            if (engine.componentManager.hasComponent<SiegeEngine>(option.target))
            {
                auto targetSE_C = &engine.componentManager.getComponent<SiegeEngine>(option.target);

////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << "AI uses: "
        << option.amount
        << " of "
        << unitCategory_to_string(actorU_C->category)
        << " for "
        << option.amount / targetSE_C->cost
        << " of "
        << unitCategory_to_string(targetU_C->category)
        );
#endif
}
////////////////////////////////////////////////////////////////////////

                onUse.invoke(option.actor, option.target, option.amount);
                CombatFunctions::use(option.amount/targetSE_C->cost, actorU_C, targetSE_C);

                std::shared_ptr<event_t::handle_t> handle = std::make_shared<event_t::handle_t>();
                *handle = turnEnd.addListener([=](){
                    CombatFunctions::reset(targetU_C, option.amount / targetSE_C->cost);
                    turnEnd.removeListener(*handle);
                });
            }else
            {
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << "AI schedules attack: "
        << unitCategory_to_string(actorU_C->category)
        << " targets "
        << unitCategory_to_string(targetU_C->category)
        << " with "
        << option.amount
        );
#endif
}
////////////////////////////////////////////////////////////////////////

                scheduleAttack(option.actor, option.target, option.amount);
                actorU_C->availableAmount -= option.amount;
            }
        }
    }
}

void CombatController::scheduleAttack(guid_t attacker, guid_t target, int amount)
{
    if (!checkIfEntityHasComponent<Unit>(attacker, target)) throw("CombatController::scheduleAttack Missing unit_C");
    auto attackerU_C = &engine.componentManager.getComponent<Unit>(attacker);
    auto targetU_C = &engine.componentManager.getComponent<Unit>(target);

    onBeforeAttack.invoke(attacker, target, amount);
    engine.actionRegister.scheduleAction(attackerU_C->speed,[=] ()
    {
        onAttack.invoke(attacker, target, amount);
        CombatFunctions::takeDamage(targetU_C, CombatFunctions::attack(attackerU_C, amount));

        std::shared_ptr<event_t::handle_t> handle = std::make_shared<event_t::handle_t>();
        *handle = turnEnd.addListener([=](){
            CombatFunctions::reset(attackerU_C, amount);
            onAfterAttack.invoke(attacker, target, amount);

            turnEnd.removeListener(*handle);
        });

////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << unitCategory_to_string(attackerU_C->category)
        <<" attacked "
        << unitCategory_to_string(targetU_C->category)
        << " with "
        << amount
        );
#endif
}
////////////////////////////////////////////////////////////////////////
    });
}

// void CombatController::handleTurn()
// {
//     for (auto guid : {pInf_E, pArc_E, pCat_E, eInf_E, eArc_E, eCat_E})
//     {
//         if (!engine.componentManager.hasComponent<Unit>(guid))
//         {
//
//         }
//     }
//     if (newTurn)
//     {
//         turnStart.invoke();
//         newTurn = false;
//     }
//     if (endOfTurn)
//     {
//         endOfTurn = engine.actionRegister.advance();
//     }
//     if (!checkIfEntityHasComponent<Unit>(pInf_E, pArc_E, pCat_E, eInf_E, eArc_E, eCat_E)) return;
//     if (!justDied)
//     {
//         auto pInfU_C = engine.componentManager.getComponent<Unit>(pInf_E);
//         auto pArcU_C = engine.componentManager.getComponent<Unit>(pArc_E);
//         auto pCatU_C = engine.componentManager.getComponent<Unit>(pCat_E);
//
//         auto eInfU_C = engine.componentManager.getComponent<Unit>(eInf_E);
//         auto eArcU_C = engine.componentManager.getComponent<Unit>(eArc_E);
//         auto eCatU_C = engine.componentManager.getComponent<Unit>(eCat_E);
//
//         if (pInfU_C.totalAmount <= 0 && pArcU_C.totalAmount <= 0 && pCatU_C.totalAmount <= 0)
//         {
//             justDied = true;
//             playerDead.invoke();
//         }else if (eInfU_C.totalAmount <= 0 && eArcU_C.totalAmount <= 0 && eCatU_C.totalAmount <= 0)
//         {
//             justDied = true;
//             enemyDead.invoke();
//         }
//     }
// }

