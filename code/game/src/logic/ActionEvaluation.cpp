//
// Created by Julian Kalb on 19/07/2025.
//

#include "ActionEvaluation.h"
#include "CombatFunctions.h"
#include "../components/unitTypes/Infantry.h"
#include "../components/unitTypes/Archer.h"
#include "../components/unitTypes/Catapult.h"

std::vector<gl3::Option> gl3::ActionEvaluation::generateAttackOptions()
{
    std::vector<Option> options;
    auto calcOption = [&](Unit* attacker, UnitCategory targetType, Unit* target, int amount)
    {
        if (attacker->availableAmount < amount) return;

        float predictedDamage = CombatFunctions::attack(attacker, amount);
        float targetHP = getTargetHP(target);
        float priority = getCategoryPriority(targetType);
        float score = (predictedDamage / targetHP) * priority;

        options.push_back({attacker, target, amount, score});
    };

    for (int percent : {100, 50, 25})
    {
        int amount = eInfU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOption(eInfU_C, UnitCategory::INFANTRY, pInfU_C, amount);
        calcOption(eInfU_C, UnitCategory::ARCHER, pArcU_C, amount);
        calcOption(eInfU_C, UnitCategory::CATAPULT, pCatU_C, amount);
    }
    for (int percent : {100, 50, 25})
    {
        int amount = eArcU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOption(eArcU_C, UnitCategory::INFANTRY, pInfU_C, amount);
        calcOption(eArcU_C, UnitCategory::ARCHER, pArcU_C, amount);
        calcOption(eArcU_C, UnitCategory::CATAPULT, pCatU_C, amount);
    }
    for (int percent : {100, 50, 25})
    {
        int amount = eCatU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOption(eCatU_C, UnitCategory::INFANTRY, pInfU_C, amount);
        calcOption(eCatU_C, UnitCategory::ARCHER, pArcU_C, amount);
        calcOption(eCatU_C, UnitCategory::CATAPULT, pCatU_C, amount);
    }
    std::sort(options.begin(), options.end(), [](const Option& a, const Option& b)
    {
        return a.score > b.score;
    });
    return options;
}

float gl3::ActionEvaluation::getTargetHP(Unit* unit)
{
    return unit->hpValue * unit->totalAmount;
}