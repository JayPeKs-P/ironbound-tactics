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
            selectionEvent.attack.addListener([&](Category selP, int amount, Category selE)
            {
                if (selP == Category::INFANTRY)
                {
                    chooseAttackTarget(pInf_C, selE, amount);
                }else if (selP == Category::ARCHER)
                {
                    chooseAttackTarget(pArc_C, selE, amount);
                }else if (selP == Category::CATAPULT)
                {
                    chooseAttackTarget(pCat_C, selE, amount);
                }
            });
            selectionEvent.use.addListener([&](Category selP, int amount, Category selPTarget)
            {
                if (selP == Category::INFANTRY)
                {
                    if (selPTarget == Category::CATAPULT)
                    {
                        CombatFunctions::use(amount, pInf_C, pCat_C);
                        std::cout << "Catapult" << std::endl;
                        engine.actionRegister.scheduleAction(1,[=]()
                        {
                            CombatFunctions::reset(pCat_C, amount);
                        });
                    }else if (selPTarget == Category::ASSAULT_COVER)
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
    auto &infContainer = game.componentManager.getContainer<Infantry>();
    auto &arcContainer = game.componentManager.getContainer<Archer>();
    auto &catContainer = game.componentManager.getContainer<Catapult>();
    for (auto &[owner, _] : infContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pInf_C = &game.componentManager.getComponent<Infantry>(owner);
            CombatFunctions::setAmount(pInf_C, 10);
        }
        else
        {
            eInf_C = &game.componentManager.getComponent<Infantry>(owner);
            CombatFunctions::setAmount(eInf_C, 20);
        }
    }
    for (auto &[owner, _] : arcContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pArc_C = &game.componentManager.getComponent<Archer>(owner);
            CombatFunctions::setAmount(pArc_C, 15);
        }
        else
        {
            eArc_C = &game.componentManager.getComponent<Archer>(owner);
            CombatFunctions::setAmount(eArc_C, 10);
        }
    }
    for (auto &[owner, _] : catContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pCat_C = &game.componentManager.getComponent<Catapult>(owner);
            CombatFunctions::setAmount(pCat_C, 5);
        }
        else
        {
            eCat_C = &game.componentManager.getComponent<Catapult>(owner);
            CombatFunctions::setAmount(eCat_C, 3);
        }
    }
}

void CombatController::chooseAttackTarget(Unit* unit, Category selection, int amount)
{
        if (selection == Category::INFANTRY)
        {
            engine.actionRegister.scheduleAction(unit->speed,
                [=] ()
            {
                CombatFunctions::takeDamage(eInf_C, CombatFunctions::attack(unit, amount));
                CombatFunctions::reset(unit, amount);
            });
        }
        if (selection == Category::ARCHER)
        {
            engine.actionRegister.scheduleAction(unit->speed,
                [=] ()
            {
                CombatFunctions::takeDamage(eArc_C, CombatFunctions::attack(unit, amount));
                CombatFunctions::reset(unit, amount);
            });
        }
        if (selection == Category::CATAPULT)
        {
            engine.actionRegister.scheduleAction(unit->speed,
                [=] ()
            {
                CombatFunctions::takeDamage(eCat_C, CombatFunctions::attack(unit, amount));
                CombatFunctions::reset(unit, amount);
            });
        }
    unit->availableAmount -= amount;
}

void CombatController::chooseAttackTarget(SiegeEngine* siege, Category selection, int amount)
{
        if (selection == Category::INFANTRY)
        {
            engine.actionRegister.scheduleAction(siege->speed,
                [=] ()
            {
                CombatFunctions::takeDamage(eInf_C, CombatFunctions::attack(siege, amount));
                CombatFunctions::reset(siege, amount);
            });
        }
        if (selection == Category::ARCHER)
        {
            engine.actionRegister.scheduleAction(siege->speed,
                [=] ()
            {
                CombatFunctions::takeDamage(eArc_C, CombatFunctions::attack(siege, amount));
                CombatFunctions::reset(siege, amount);
            });
        }
        if (selection == Category::CATAPULT)
        {
            engine.actionRegister.scheduleAction(siege->speed,
                [=] ()
            {
                CombatFunctions::takeDamage(eCat_C, CombatFunctions::attack(siege, amount));
                CombatFunctions::reset(siege, amount);
            });
        }
    siege->availableAmount -= amount;
}

void CombatController::handleTurn()
{
    if (pInf_C->availableAmount == 0 && pArc_C->availableAmount == 0 && pCat_C->availableAmount == 0)
    {
        engine.actionRegister.advance();
    }
}
