//
// Created by Julian Kalb on 19/07/2025.
//

#include "EnemyAI.h"

#include <iostream>

#include "LibCombatFunctions.h"
#include "../components/TagComponent.h"
#include "engine/Game.h"

using namespace gl3;
EnemyAI* EnemyAI::m_pInstance = nullptr;

template<typename T, typename ...Guids>
bool checkIfEntityHasComponent(engine::Game &game, Guids... guids)
{
    return (... && game.componentManager.hasComponent<T>((guids)));
}

void EnemyAI::setGuids(engine::Game& game)
{
    game.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {
        auto &tag = game.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag == Tag::PLAYER)
        {
            switch (unit.category)
            {
            case UnitCategory::INFANTRY:
                iInfantryPlayer_ID = unit.entity();
                break;
            case UnitCategory::ARCHER:
                iArcherPlayer_ID = unit.entity();
                break;
            case UnitCategory::CATAPULT:
                iCatapultPlayer_ID = unit.entity();
                break;
            }
        }
        if (tag == Tag::ENEMY)
        {
            switch (unit.category)
            {
            case UnitCategory::INFANTRY:
                iInfantryEnemy_ID = unit.entity();
                break;
            case UnitCategory::ARCHER:
                iArcherEnemy_ID = unit.entity();
                break;
            case UnitCategory::CATAPULT:
                iCatapultEnemy_ID = unit.entity();
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
    if (!checkIfEntityHasComponent<Unit>(game, iInfantryPlayer_ID, iArcherPlayer_ID, iCatapultPlayer_ID, iInfantryEnemy_ID, iArcherEnemy_ID, iCatapultEnemy_ID))
    {
        throw("ActionEvaluation::checkIfEntityHasComponent failed");
    }
}

std::vector<Option> EnemyAI::generateOptions(engine::Game &game)
{
    if (!checkIfEntityHasComponent<Unit>(game, iInfantryPlayer_ID, iArcherPlayer_ID, iCatapultPlayer_ID, iInfantryEnemy_ID, iArcherEnemy_ID, iCatapultEnemy_ID))
    {
        throw("ActionEvaluation::checkIfEntityHasComponent failed");
    }
    auto pInfantryUnitPlayer_C = &game.componentManager.getComponent<Unit>(iInfantryPlayer_ID);
    auto pArcherUnitPlayer_C = &game.componentManager.getComponent<Unit>(iArcherPlayer_ID);
    auto pCatapultUnitPlayer_C = &game.componentManager.getComponent<Unit>(iCatapultPlayer_ID);
    auto pCatapultSiegeEnginePlayer_C = &game.componentManager.getComponent<SiegeEngine>(iCatapultPlayer_ID);

    auto pInfantryUnitEnemy_C = &game.componentManager.getComponent<Unit>(iInfantryEnemy_ID);
    auto pArcherUnitEnemy_C = &game.componentManager.getComponent<Unit>(iArcherEnemy_ID);
    auto pCatapultUnitEnemy_C = &game.componentManager.getComponent<Unit>(iCatapultEnemy_ID);
    auto pCatapultSiegeEngineEnemy_C = &game.componentManager.getComponent<SiegeEngine>(iCatapultEnemy_ID);

    std::vector<Option> options;
    auto calcOptionAttack = [&](guid_t iAttacker_ID, const Unit* pAttackerUnit_C, Unit* pTargetUnit_C, int amount)
    {

        if (pAttackerUnit_C->availableAmount < amount) return;

        float targetHP = getTargetHP(pTargetUnit_C);
        if (targetHP <= 0) return;

        auto pLibCombat = LibCombatFunctions::GetInstance(game);
        float predictedDamage = pLibCombat->attack(iAttacker_ID, amount)/pAttackerUnit_C->speed;
        float priority = getCategoryPriority(pTargetUnit_C->category);
        if (targetHP*1.1 < predictedDamage) priority = 0.9;
        float score = (targetHP / predictedDamage ) * priority;

        options.push_back({iAttacker_ID, pTargetUnit_C->entity(), amount, score});
    };
    auto calcOptionUse = [&](guid_t iTarget_ID, Unit* actor, Unit* targetU, SiegeEngine* targetSE, int amount)
    {
        int unusedTargets;
        if (actor->availableAmount < amount) return;
        if (amount < targetSE->cost) return;
        if(targetU->totalAmount*targetSE->cost <= amount)
        {
            unusedTargets = pCatapultUnitPlayer_C->totalAmount-pCatapultSiegeEnginePlayer_C->useableAmount;
        }else
        {
            unusedTargets = actor->availableAmount / pCatapultSiegeEnginePlayer_C->cost;
        }
        auto pLibCombat = LibCombatFunctions::GetInstance(game);
        float predictedDamage = pLibCombat->attack(iTarget_ID, unusedTargets) / targetU->speed;
        float score = predictedDamage; //TODO COMPARE WITH DAMAGE UNITS WOULD DEAL ON THEIR OWN
        options.push_back( {actor->entity(), targetU->entity() , amount, score} );
    };

    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = pInfantryUnitEnemy_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOptionAttack(iInfantryEnemy_ID, pInfantryUnitEnemy_C,pInfantryUnitPlayer_C, amount);
        calcOptionAttack(iInfantryEnemy_ID, pInfantryUnitEnemy_C,pArcherUnitPlayer_C, amount);
        calcOptionAttack(iInfantryEnemy_ID,pInfantryUnitEnemy_C,pCatapultUnitPlayer_C, amount);
        calcOptionUse(iInfantryEnemy_ID, pInfantryUnitEnemy_C, pCatapultUnitEnemy_C, pCatapultSiegeEngineEnemy_C, amount);
    }
    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = pArcherUnitEnemy_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOptionAttack(iArcherEnemy_ID, pArcherUnitEnemy_C,pInfantryUnitPlayer_C, amount);
        calcOptionAttack(iArcherEnemy_ID, pArcherUnitEnemy_C,pArcherUnitPlayer_C, amount);
        calcOptionAttack(iArcherEnemy_ID, pArcherUnitEnemy_C,pCatapultUnitPlayer_C, amount);
    }
    for (int percent : {100, 90, 80, 70, 60, 50, 40, 30, 20, 10})
    {
        int amount = pCatapultUnitEnemy_C->availableAmount * percent / 100;
        if (amount == 0) continue;
        calcOptionAttack(iCatapultEnemy_ID, pCatapultUnitEnemy_C,pInfantryUnitPlayer_C, amount);
        calcOptionAttack(iCatapultEnemy_ID, pCatapultUnitEnemy_C,pArcherUnitPlayer_C, amount);
        calcOptionAttack(iCatapultEnemy_ID, pCatapultUnitEnemy_C,pCatapultUnitPlayer_C, amount);
    }
    std::sort(options.begin(), options.end(), [](const Option& a, const Option& b)
    {
        return a.score > b.score;
    });
    return options;
}

float EnemyAI::getTargetHP(Unit* unit)
{
    return unit->hpValue * unit->totalAmount;
}