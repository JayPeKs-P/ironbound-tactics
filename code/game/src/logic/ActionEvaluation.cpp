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
    auto calcUnitOption = [&](Unit* attacker, Category targetType, float targetHP, int amount)
    {
        if (attacker->availableAmount < amount) return;

        float predictedDamage = CombatFunctions::attack(attacker, amount);
        float priority = getCategoryPriority(targetType);
        float score = (predictedDamage / targetHP) * priority;

        options.push_back({attacker, nullptr, targetType, amount, score});
    };
    auto calcSiegeOption = [&](SiegeEngine* attacker, Category targetType, float targetHP, int amount)
    {
        if (attacker->availableAmount < amount) return;

        float predictedDamage = CombatFunctions::attack(attacker, amount);
        float priority = getCategoryPriority(targetType);
        float score = (predictedDamage / targetHP) * priority;

        options.push_back({nullptr, attacker, targetType, amount, score});
    };

    for (int percent : {100, 50, 25})
    {
        int amount = eInf_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcUnitOption(eInf_C, Category::INFANTRY, getTargetHP(pInf_C), amount);
        calcUnitOption(eInf_C, Category::ARCHER, getTargetHP(pArc_C), amount);
        calcUnitOption(eInf_C, Category::CATAPULT, getTargetHP(pCat_C), amount);
    }
    for (int percent : {100, 50, 25})
    {
        int amount = eArc_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcUnitOption(eArc_C, Category::INFANTRY, getTargetHP(pInf_C), amount);
        calcUnitOption(eArc_C, Category::ARCHER, getTargetHP(pArc_C), amount);
        calcUnitOption(eArc_C, Category::CATAPULT, getTargetHP(pCat_C), amount);
    }
    for (int percent : {100, 50, 25})
    {
        int amount = eCat_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcSiegeOption(eCat_C, Category::INFANTRY, getTargetHP(pInf_C), amount);
        calcSiegeOption(eCat_C, Category::ARCHER, getTargetHP(pArc_C), amount);
        calcSiegeOption(eCat_C, Category::CATAPULT, getTargetHP(pCat_C), amount);
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

float gl3::ActionEvaluation::getTargetHP(SiegeEngine* siege)
{
    return siege->hpValue * siege->totalAmount;
}
