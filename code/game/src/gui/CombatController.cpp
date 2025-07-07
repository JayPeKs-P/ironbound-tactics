//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"

#include <iostream>
#include <ostream>

#include "../components/CombatSelection.h"
#include "GuiCombat.h"
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
                    if (selE == Category::INFANTRY)takeDamage(eInf_C, attack(pInf_C, amount));
                    if (selE == Category::ARCHER) std::cout << "Archer: "<< amount << std::endl;
                    if (selE == Category::CATAPULT);
                }else if (selP == Category::ARCHER)
                {
                    if (selE == Category::INFANTRY) std::cout << "Infantry: "<< amount << std::endl;
                    if (selE == Category::ARCHER) std::cout << "Archer: "<< amount << std::endl;
                    if (selE == Category::CATAPULT);
                }else if (selP == Category::CATAPULT)
                {
                    if (selE == Category::INFANTRY) std::cout << "Infantry: "<< amount << std::endl;
                    if (selE == Category::ARCHER) std::cout << "Archer: "<< amount << std::endl;
                    if (selE == Category::CATAPULT);
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

void CombatController::handleTurn(bool newRound)
{
    // float pHP = 0;
    // for (auto& unit: playerUnits)
    // {
    //     pHP += unit->getLifePoints();
    // }
    // std::cout<<"Player HP: "<<pHP<<std::endl;
    // float eHP = 0;
    // for (auto& unit: enemyUnits)
    // {
    //     eHP += unit->getLifePoints();
    // }
    // std::cout<<"Enemy HP: "<<eHP<<std::endl;
    // std::cout<<"Attack with Inf? [y/n]"<<std::endl;
    // char input;
    // std::cin>>input;
    // if (input=='y')
    // {
    //     std::cout<<"Target? [a/i]"<<std::endl;
    //     std::cin>>input;
    //     if (input=='a')
    //     {
    //         enemyUnits[1]->takeDamage(playerUnits[0]->attack());
    //     }
    //     if (input=='i')
    //     {
    //         enemyUnits[0]->takeDamage(playerUnits[0]->attack());
    //     }
    // }

}
