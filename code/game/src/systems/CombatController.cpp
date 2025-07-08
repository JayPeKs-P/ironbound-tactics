//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"

#include <iostream>
#include <ostream>

#include "../gui/GuiCombat.h"
#include "../components/TagComponent.h"


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
                    std::cout << "Catapult attack" << std::endl;
                    chooseAttackTarget(pCat_C, selE, amount);
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
            auto handle = onTurnEnd.addListener([=]()
            {
                takeDamage(eInf_C, attack(unit, amount));
            });
            listenersTurnEnd.push_back(handle);
        }
        if (selection == Category::ARCHER)
        {
            auto handle = onTurnEnd.addListener([=]()
            {
                takeDamage(eArc_C, attack(unit, amount));
            });
            listenersTurnEnd.push_back(handle);
        }
        if (selection == Category::CATAPULT)
        {
            auto handle = onTurnEnd.addListener([=]()
            {
                takeDamage(eCat_C, attack(unit, amount));
            });
            listenersTurnEnd.push_back(handle);
        }
    unit->availableAmount -= amount;
}

void CombatController::setAmount(Unit* unit, int amount)
{
    unit->totalAmount = amount;
    unit->lifetimeMaxAmount = amount;
    unit->availableAmount = amount;
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
    unit->availableAmount = unit->totalAmount;
    return totalDamage;
}

void CombatController::takeDamage(Unit* unit, float damage)
{
    float amountDead = damage/unit->unitLPValue;
    unit->totalAmount -= amountDead;
    // float differenceAD = damage - armorValue;
    // armorValue -= damage/20;
    // if (differenceAD > 0)
    // {
    //    lifePoints -= differenceAD;
    // }
    // return lifePoints;
}

void CombatController::handleTurn()
{
    if (pInf_C->availableAmount == 0 && pArc_C->availableAmount == 0 && pCat_C->availableAmount == 0)
    {
        onTurnEnd.invoke();
        for (auto& handle : listenersTurnEnd)
        {
            onTurnEnd.removeListener(handle);
        }
        listenersTurnEnd.clear();
    }
}
