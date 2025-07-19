//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"

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
                        use(amount);
                        std::cout << "Catapult" << std::endl;
                        engine.actionRegister.scheduleAction(1,[=]()
                        {
                            reset(pCat_C, amount);
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
            setAmount(pInf_C, 10);
        }
        else
        {
            eInf_C = &game.componentManager.getComponent<Infantry>(owner);
            setAmount(eInf_C, 20);
        }
    }
    for (auto &[owner, _] : arcContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pArc_C = &game.componentManager.getComponent<Archer>(owner);
            setAmount(pArc_C, 15);
        }
        else
        {
            eArc_C = &game.componentManager.getComponent<Archer>(owner);
            setAmount(eArc_C, 10);
        }
    }
    for (auto &[owner, _] : catContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pCat_C = &game.componentManager.getComponent<Catapult>(owner);
            setAmount(pCat_C, 5);
        }
        else
        {
            eCat_C = &game.componentManager.getComponent<Catapult>(owner);
            setAmount(eCat_C, 3);
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
                takeDamage(eInf_C, attack(unit, amount));
                reset(unit, amount);
            });
        }
        if (selection == Category::ARCHER)
        {
            engine.actionRegister.scheduleAction(unit->speed,
                [=] ()
            {
                takeDamage(eArc_C, attack(unit, amount));
                reset(unit, amount);
            });
        }
        if (selection == Category::CATAPULT)
        {
            engine.actionRegister.scheduleAction(unit->speed,
                [=] ()
            {
                takeDamage(eCat_C, attack(unit, amount));
                reset(unit, amount);
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
                takeDamage(eInf_C, attack(siege, amount));
                reset(siege, amount);
            });
        }
        if (selection == Category::ARCHER)
        {
            engine.actionRegister.scheduleAction(siege->speed,
                [=] ()
            {
                takeDamage(eArc_C, attack(siege, amount));
                reset(siege, amount);
            });
        }
        if (selection == Category::CATAPULT)
        {
            engine.actionRegister.scheduleAction(siege->speed,
                [=] ()
            {
                takeDamage(eCat_C, attack(siege, amount));
                reset(siege, amount);
            });
        }
    siege->availableAmount -= amount;
}

void CombatController::setAmount(Unit* unit, int value)
{
    unit->totalAmount = value;
    unit->totalAmountLastFrame = value;
    unit->lifetimeMaxAmount = value;
    unit->availableAmount = value;
}

void CombatController::setAmount(SiegeEngine* siege, int value)
{
    siege->totalAmount = value;
    siege->totalAmountLastFrame = value;
    siege->lifetimeMaxAmount = value;
}

float CombatController::attack(Unit* unit, int amount)
{
    float totalDamage = 0;
    for (int i = 0; i < amount; i++)
    {
        int hitRoll = dist(rng);
        if (hitRoll >= unit->accuracy)
        {
            continue;
        }
        float damage = unit->attackValue;

        int critRoll = dist(rng);
        if (critRoll < unit->critChance)
        {
            damage *= unit->critMultiplier;
        }
        totalDamage += damage;
    }
    return totalDamage;
}

float CombatController::attack(SiegeEngine* siege, int amount)
{
    float totalDamage = 0;
    for (int i = 0; i < amount; i++)
    {
        int hitRoll = dist(rng);
        if (hitRoll >= siege->accuracy)
        {
            continue;
        }
        float damage = siege->attackValue;

        int critRoll = dist(rng);
        if (critRoll < siege->critChance)
        {
            damage *= siege->critMultiplier;
        }
        totalDamage += damage;
    }
    return totalDamage;
}

void CombatController::takeDamage(Unit* unit, float damage)
{
    float amountDead = damage/unit->hpValue;
    unit->totalAmount -= static_cast<int>(amountDead);
    // float differenceAD = damage - armorValue;
    // armorValue -= damage/20;
    // if (differenceAD > 0)
    // {
    //    lifePoints -= differenceAD;
    // }
    // return lifePoints;
}

void CombatController::takeDamage(SiegeEngine* siege, float damage)
{
     float amountDead = damage/siege->hpValue;
    siege->totalAmount -= static_cast<int>(amountDead);
}

void CombatController::use(int amount)
{
    pCat_C->useableAmount += amount;
    pInf_C->totalAmount -= amount * pCat_C->cost;
    pInf_C->availableAmount -= amount * pCat_C->cost;

}

void CombatController::reset(Unit* unit, int amount)
{
    unit->availableAmount += amount;
}

void CombatController::reset(SiegeEngine* siege, int amount)
{
    siege->availableAmount += amount;
}

void CombatController::handleTurn()
{
    if (pInf_C->availableAmount == 0 && pArc_C->availableAmount == 0 && pCat_C->availableAmount == 0)
    {
        engine.actionRegister.advance();
    }
}
