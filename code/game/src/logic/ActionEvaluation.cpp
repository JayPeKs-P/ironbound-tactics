//
// Created by Julian Kalb on 19/07/2025.
//

#include "ActionEvaluation.h"

#include <iostream>

#include "CombatFunctions.h"
#include "../components/TagComponent.h"
#include "engine/Game.h"

using namespace gl3;
guid_t ActionEvaluation::pInf_E = -1;
guid_t ActionEvaluation::pArc_E = -1;
guid_t ActionEvaluation::pCat_E = -1;

guid_t ActionEvaluation::eInf_E = -1;
guid_t ActionEvaluation::eArc_E = -1;
guid_t ActionEvaluation::eCat_E = -1;

template<typename T, typename ...Guids>
bool checkIfEntityHasComponent(engine::Game &game, Guids... guids)
{
    return (... && game.componentManager.hasComponent<T>((guids)));
}

void ActionEvaluation::setGuids(engine::Game& game)
{
    game.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {
        auto &tag = game.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag == Tag::PLAYER)
        {
            switch (unit.category)
            {
            case UnitCategory::INFANTRY:
                pInf_E = unit.entity();
                break;
            case UnitCategory::ARCHER:
                pArc_E = unit.entity();
                break;
            case UnitCategory::CATAPULT:
                pCat_E = unit.entity();
                break;
            }
        }
        if (tag == Tag::ENEMY)
        {
            switch (unit.category)
            {
            case UnitCategory::INFANTRY:
                eInf_E = unit.entity();
                break;
            case UnitCategory::ARCHER:
                eArc_E = unit.entity();
                break;
            case UnitCategory::CATAPULT:
                eCat_E = unit.entity();
                break;
            }
        }
    });
    // std::cout << pInf_E << std::endl;
    // std::cout << pArc_E << std::endl;
    // std::cout << pCat_E << std::endl;
    // std::cout << eInf_E << std::endl;
    // std::cout << eArc_E << std::endl;
    // std::cout << eCat_E << std::endl;
    if (!checkIfEntityHasComponent<Unit>(game, pInf_E, pArc_E, pCat_E, eInf_E, eArc_E, eCat_E))
    {
        throw("ActionEvaluation::checkIfEntityHasComponent failed");
    }
}

std::vector<Option> ActionEvaluation::generateOptions(engine::Game &game)
{
    if (!checkIfEntityHasComponent<Unit>(game, pInf_E, pArc_E, pCat_E, eInf_E, eArc_E, eCat_E))
    {
        throw("ActionEvaluation::checkIfEntityHasComponent failed");
    }
    auto pInfU_C = &game.componentManager.getComponent<Unit>(pInf_E);
    auto pArcU_C = &game.componentManager.getComponent<Unit>(pArc_E);
    auto pCatU_C = &game.componentManager.getComponent<Unit>(pCat_E);
    auto pCatSE_C = &game.componentManager.getComponent<SiegeEngine>(pCat_E);

    auto eInfU_C = &game.componentManager.getComponent<Unit>(eInf_E);
    auto eArcU_C = &game.componentManager.getComponent<Unit>(eArc_E);
    auto eCatU_C = &game.componentManager.getComponent<Unit>(eCat_E);
    auto eCatSE_C = &game.componentManager.getComponent<SiegeEngine>(eCat_E);

    std::vector<Option> options;
    auto calcOptionAttack = [&](Unit* attacker,Unit* target, int amount)
    {

        if (attacker->availableAmount < amount) return;

        float targetHP = getTargetHP(target);
        if (targetHP <= 0) return;

        float predictedDamage = CombatFunctions::attack(attacker, amount)/attacker->speed;
        float priority = getCategoryPriority(target->category);
        if (targetHP*1.1 < predictedDamage) priority = 0.9;
        float score = (targetHP / predictedDamage ) * priority;

        options.push_back({attacker->entity(), target->entity(), amount, score});
    };
    auto calcOptionUse = [&](Unit* actor, Unit* targetU, SiegeEngine* targetSE, int amount)
    {
        int unusedTargets;
        if (actor->availableAmount < amount) return;
        if (amount < targetSE->cost) return;
        if(targetU->totalAmount*targetSE->cost <= amount)
        {
            unusedTargets = pCatU_C->totalAmount-pCatSE_C->useableAmount;
        }else
        {
            unusedTargets = actor->availableAmount / pCatSE_C->cost;
        }
        float predictedDamage = CombatFunctions::attack(targetU, unusedTargets) / targetU->speed;
        float score = predictedDamage; //TODO COMPARE WITH DAMAGE UNITS WOULD DEAL ON THEIR OWN
        options.push_back( {actor->entity(), targetU->entity() , amount, score} );
    };

    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = eInfU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOptionAttack(eInfU_C,pInfU_C, amount);
        calcOptionAttack(eInfU_C,pArcU_C, amount);
        calcOptionAttack(eInfU_C,pCatU_C, amount);
        calcOptionUse(eInfU_C, eCatU_C, eCatSE_C, amount);
    }
    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = eArcU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOptionAttack(eArcU_C,pInfU_C, amount);
        calcOptionAttack(eArcU_C,pArcU_C, amount);
        calcOptionAttack(eArcU_C,pCatU_C, amount);
    }
    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = eCatU_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOptionAttack(eCatU_C,pInfU_C, amount);
        calcOptionAttack(eCatU_C,pArcU_C, amount);
        calcOptionAttack(eCatU_C,pCatU_C, amount);
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