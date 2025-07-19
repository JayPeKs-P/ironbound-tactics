//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"
#include "../logic/CombatFunctions.h"

#include <iostream>
#include <ostream>

#include "../gui/GuiCombat.h"
#include "../components/TagComponent.h"
#include "engine/logic/ActionRegistry.h"


using namespace gl3;

CombatController::CombatController(engine::Game &game):
    System(game)
{
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
                        std::cout << "Catapult" << std::endl;
                        engine.actionRegister.scheduleAction(1,[=]()
                        {
                            CombatFunctions::reset(pCatU_C, amount);
                        });
                    }else if (selPTarget == UnitCategory::ASSAULT_COVER)
                    {

                    }
                }
            });
        }
    }
}

CombatController::~CombatController()
{
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
}

void CombatController::chooseAttackTarget(Unit* attacker, const UnitCategory &target, const int &amount)
{
        if (target == UnitCategory::INFANTRY)
        {
            scheduleAttack(attacker, eInfU_C, amount);
        }
        if (target == UnitCategory::ARCHER)
        {
            scheduleAttack(attacker, eArcU_C, amount);
        }
        if (target == UnitCategory::CATAPULT)
        {
            scheduleAttack(attacker, eCatU_C, amount);
        }
    attacker->availableAmount -= amount;
}

void CombatController::scheduleAttack(Unit* attacker, Unit* target, int amount)
{
    engine.actionRegister.scheduleAction(attacker->speed,[=] ()
    {
        CombatFunctions::takeDamage(target, CombatFunctions::attack(attacker, amount));
        CombatFunctions::reset(attacker, amount);
    });
}

//
// void CombatController::chooseAttackTarget(SiegeEngine* siege, UnitCategory selection, int amount)
// {
//         if (selection == UnitCategory::INFANTRY)
//         {
//             engine.actionRegister.scheduleAction(siege->speed,
//                 [=] ()
//             {
//                 CombatFunctions::takeDamage(eInf_C, CombatFunctions::attack(siege, amount));
//                 CombatFunctions::reset(siege, amount);
//             });
//         }
//         if (selection == UnitCategory::ARCHER)
//         {
//             engine.actionRegister.scheduleAction(siege->speed,
//                 [=] ()
//             {
//                 CombatFunctions::takeDamage(eArc_C, CombatFunctions::attack(siege, amount));
//                 CombatFunctions::reset(siege, amount);
//             });
//         }
//         if (selection == UnitCategory::CATAPULT)
//         {
//             engine.actionRegister.scheduleAction(siege->speed,
//                 [=] ()
//             {
//                 CombatFunctions::takeDamage(eCat_C, CombatFunctions::attack(siege, amount));
//                 CombatFunctions::reset(siege, amount);
//             });
//         }
//     siege->availableAmount -= amount;
// }

void CombatController::handleTurn()
{
    if (pInfU_C->availableAmount == 0 && pArcU_C->availableAmount == 0 && pCatU_C->availableAmount == 0)
    {
        engine.actionRegister.advance();
    }
}
