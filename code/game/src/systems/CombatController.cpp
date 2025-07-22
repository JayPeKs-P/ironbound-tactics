//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"
#include "../logic/CombatFunctions.h"
#include "../logic/ActionEvaluation.h"

#include <iostream>
#include <ostream>

#include "../gui/GuiCombat.h"
#include "../components/TagComponent.h"
#include "engine/logic/ActionRegistry.h"

#include "../components/unitTypes/UnitCategory.h"
#include "engine/util/Debug.h"


using namespace gl3;

CombatController::eventAttack_t CombatController::onBeforeAttack;
CombatController::eventAttack_t CombatController::onAttack;
CombatController::eventAttack_t CombatController::onAfterAttack;
engine::events::Event<CombatController, int, Unit*, SiegeEngine*> CombatController::onUse;

CombatController::CombatController(engine::Game &game):
    System(game)
{
////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_LOG
    onBeforeAttack.addListener([&](Unit* unit, Unit* other, int i)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onBeforeAttack'"
            );
    });
    onAttack.addListener([&](Unit* unit, Unit* other, int i)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onAttack'"
            );
    });
    onAfterAttack.addListener([&](Unit* unit, Unit* other, int i)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onAfterAttack'"
            );
    });
    onUse.addListener([&](int i, Unit* unit, SiegeEngine* other)
    {
        DEBUG_LOG(
            << "TRIGGERED EVENT: 'onUse'"
            );
    });
#endif
////////////////////////////////////////////////////////////////////////

    turnStart.addListener([=]()
    {
        runEnemyTurn();
    });
    for (auto& [owner, _] : engine.componentManager.getContainer<CombatSelection<GuiCombat>>())
    {
        if (game.componentManager.hasComponent<CombatSelection<GuiCombat>>(owner))
        {
            auto& selectionEvent = engine.componentManager.getComponent<CombatSelection<GuiCombat>>(owner);
            selectionEvent.attack.addListener([&](UnitCategory selP, int amount, UnitCategory selE)
            {
                if (selP == UnitCategory::INFANTRY)
                {
                    chooseAttackTarget(pInfU_C, selE, amount);
                }else if (selP == UnitCategory::ARCHER)
                {
                    chooseAttackTarget(pArcU_C, selE, amount);
                }else if (selP == UnitCategory::CATAPULT)
                {
                    chooseAttackTarget(pCatU_C, selE, amount);
                }
            });
            selectionEvent.use.addListener([&](UnitCategory selP, int amount, UnitCategory selPTarget)
            {
                if (selP == UnitCategory::INFANTRY)
                {
                    if (selPTarget == UnitCategory::CATAPULT)
                    {
                        CombatFunctions::use(amount, pInfU_C, pCatSE_C);
                        std::shared_ptr<event_t::handle_t> handle = std::make_shared<event_t::handle_t>();
                        *handle = turnEnd.addListener([=](){
                            CombatFunctions::reset(pCatU_C, amount);
                            turnEnd.removeListener(*handle);
                        });
                        // engine.actionRegister.scheduleAction(1,[=]()
                        // {
                        //     CombatFunctions::reset(pCatU_C, amount);
                        // });
                    }else if (selPTarget == UnitCategory::ASSAULT_COVER)
                    {

                    }
                }
            });
        }
    }
}

void CombatController::init(engine::Game &game)
{
    game.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {
        auto &tag = game.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag == Tag::PLAYER)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                pInfU_C = &unit;
                CombatFunctions::setAmount(pInfU_C, 20);
            }else if (unit.category == UnitCategory::ARCHER)
            {
                pArcU_C = &unit;
                CombatFunctions::setAmount(pArcU_C, 20);
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                pCatU_C = &unit;
                pCatSE_C = &game.componentManager.getComponent<SiegeEngine>(unit.entity());
                CombatFunctions::setAmount(pCatU_C, 5);
            }
        }else if (tag == Tag::ENEMY)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                eInfU_C = &unit;
                CombatFunctions::setAmount(eInfU_C, 20);
            }else if (unit.category == UnitCategory::ARCHER)
            {
                eArcU_C = &unit;
                CombatFunctions::setAmount(eArcU_C, 20);
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                eCatU_C = &unit;
                eCatSE_C = &game.componentManager.getComponent<SiegeEngine>(unit.entity());
                CombatFunctions::setAmount(eCatU_C, 5);
            }
        }
    });
    ActionEvaluation::setPointers(game);
}

void CombatController::chooseAttackTarget(Unit* attacker, const UnitCategory &target, const int &amount)
{
    switch (target)
    {
        case UnitCategory::INFANTRY:
            scheduleAttack(attacker, eInfU_C, amount);
            break;
        case UnitCategory::ARCHER:
            scheduleAttack(attacker, eArcU_C, amount);
            break;
        case UnitCategory::CATAPULT:
            scheduleAttack(attacker, eCatU_C, amount);
            break;
    }

////////////////////////////////////////////////////////////////////////
    DEBUG_LOG(
        << "Player schedules attack: "
        << unitCategory_to_string(attacker->category)
        <<" targets "
        << unitCategory_to_string(target)
        << " with "
        << amount
        );
////////////////////////////////////////////////////////////////////////

    attacker->availableAmount -= amount;
}

void CombatController::runEnemyTurn()
{
    auto options = ActionEvaluation::generateOptions();
    for (auto& option: options)
    {
        if (option.actor->availableAmount >= option.amount)
        {
            if (option.siege != nullptr)
            {

////////////////////////////////////////////////////////////////////////
                DEBUG_LOG(
                    << "AI uses: "
                    << option.amount
                    << " of "
                    << unitCategory_to_string(option.actor->category)
                    << " for "
                    << option.amount / option.siege->cost
                    << " of "
                    << unitCategory_to_string(option.target->category)
                    );
////////////////////////////////////////////////////////////////////////

                onUse.invoke(option.amount, option.actor, option.siege);
                CombatFunctions::use(option.amount/option.siege->cost, option.actor, option.siege);

                std::shared_ptr<event_t::handle_t> handle = std::make_shared<event_t::handle_t>();
                *handle = turnEnd.addListener([=](){
                    CombatFunctions::reset(option.target, option.amount / option.siege->cost);
                    turnEnd.removeListener(*handle);
                });
            }else
            {
////////////////////////////////////////////////////////////////////////
                DEBUG_LOG(
                    << "AI schedules attack: "
                    << unitCategory_to_string(option.actor->category)
                    << " targets "
                    << unitCategory_to_string(option.target->category)
                    << " with "
                    << option.amount
                    );
////////////////////////////////////////////////////////////////////////

                scheduleAttack(option.actor, option.target, option.amount);
                option.actor->availableAmount -= option.amount;
            }
        }
    }
}

void CombatController::scheduleAttack(Unit* attacker, Unit* target, int amount)
{
    onBeforeAttack.invoke(attacker, nullptr, amount);
    engine.actionRegister.scheduleAction(attacker->speed,[=] ()
    {
        onAttack.invoke(attacker, target, amount);
        CombatFunctions::takeDamage(target, CombatFunctions::attack(attacker, amount));

        std::shared_ptr<event_t::handle_t> handle = std::make_shared<event_t::handle_t>();
        *handle = turnEnd.addListener([=](){
            CombatFunctions::reset(attacker, amount);
            onAfterAttack.invoke(attacker, nullptr, amount);

            turnEnd.removeListener(*handle);
        });

////////////////////////////////////////////////////////////////////////
        DEBUG_LOG(
            << unitCategory_to_string(attacker->category)
            <<" attacked "
            << unitCategory_to_string(target->category)
            << " with "
            << amount
            );
////////////////////////////////////////////////////////////////////////
    });
}

void CombatController::handleTurn()
{
    if (endOfTurn)
    {
////////////////////////////////////////////////////////////////////////
        DEBUG_LOG(
            << "=======| This is turn: "
            << turnCount
            << " |======="
            );
////////////////////////////////////////////////////////////////////////

        turnStart.invoke();
        endOfTurn = false;
    }
    if (pInfU_C->availableAmount == 0 && pArcU_C->availableAmount == 0 && pCatU_C->availableAmount == 0)
    {
        engine.actionRegister.advance();
        endOfTurn = true;
    }
    if (endOfTurn)
    {
        turnEnd.invoke();

        std::shared_ptr<event_t::handle_t> handle = std::make_shared<event_t::handle_t>();
        *handle = turnStart.addListener([=](){
            runEnemyTurn();
            turnStart.removeListener(*handle);
        });

////////////////////////////////////////////////////////////////////////
        DEBUG_LOG(
            << "=======| End of turn: "
            << turnCount
            << " |======="
            );
////////////////////////////////////////////////////////////////////////

        turnCount++;
    }
}
