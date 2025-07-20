//
// Created by Julian Kalb on 19/07/2025.
//

#include "ActionEvaluation.h"

#include <iostream>

#include "CombatFunctions.h"
#include "../components/TagComponent.h"
#include "engine/Game.h"

using namespace gl3;
Unit* ActionEvaluation::pInfU_C = nullptr;
Unit* ActionEvaluation::pArcU_C = nullptr;
Unit* ActionEvaluation::pCatU_C = nullptr;
SiegeEngine* ActionEvaluation::pCatSE_C = nullptr;

Unit* ActionEvaluation::eInfU_C = nullptr;
Unit* ActionEvaluation::eArcU_C = nullptr;
Unit* ActionEvaluation::eCatU_C = nullptr;
SiegeEngine* ActionEvaluation::eCatSE_C = nullptr;

void ActionEvaluation::setPointers(engine::Game& game)
{
    game.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {
        auto &tag = game.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag == Tag::PLAYER)
        {
            switch (unit.category)
            {
            case UnitCategory::INFANTRY:
                pInfU_C = &unit;
                break;
            case UnitCategory::ARCHER:
                pArcU_C = &unit;
                break;
            case UnitCategory::CATAPULT:
                pCatU_C = &unit;
                pCatSE_C = &game.componentManager.getComponent<SiegeEngine>(unit.entity());
                break;
            }
        }
        if (tag == Tag::ENEMY)
        {
            switch (unit.category)
            {
            case UnitCategory::INFANTRY:
                eInfU_C = &unit;
                break;
            case UnitCategory::ARCHER:
                eArcU_C = &unit;
                break;
            case UnitCategory::CATAPULT:
                eCatU_C = &unit;
                eCatSE_C = &game.componentManager.getComponent<SiegeEngine>(unit.entity());
                break;
            }
        }
    });
}

std::vector<Option> ActionEvaluation::generateAttackOptions()
{
    std::vector<Option> options;
    auto calcOption = [&](Unit* attacker,Unit* target, int amount)
    {
        if (attacker->availableAmount < amount) return;

        float predictedDamage = CombatFunctions::attack(attacker, amount)/attacker->speed;
        float targetHP = getTargetHP(target);
        float priority = getCategoryPriority(target->category);
        float score = (predictedDamage / targetHP) * priority;

        options.push_back({attacker, target, amount, score});
    };

    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = eInfU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOption(eInfU_C,pInfU_C, amount);
        calcOption(eInfU_C,pArcU_C, amount);
        calcOption(eInfU_C,pCatU_C, amount);
    }
    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = eArcU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOption(eArcU_C,pInfU_C, amount);
        calcOption(eArcU_C,pArcU_C, amount);
        calcOption(eArcU_C,pCatU_C, amount);
    }
    for (int percent = 5; percent < 100; percent+=5)
    {
        int amount = eCatU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOption(eCatU_C,pInfU_C, amount);
        calcOption(eCatU_C,pArcU_C, amount);
        calcOption(eCatU_C,pCatU_C, amount);
    }
    std::sort(options.begin(), options.end(), [](const Option& a, const Option& b)
    {
        return a.score > b.score;
    });
    return options;
}

float ActionEvaluation::getTargetHP(Unit* unit)
{
    return unit->hpValue * unit->totalAmount;
}