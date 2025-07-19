//
// Created by Julian Kalb on 19/07/2025.
//

#include "CombatFunctions.h"

namespace gl3 {
void CombatFunctions::setAmount(Unit* unit, int value)
{
    switch (unit->category)
    {
    case UnitCategory::CATAPULT:
        unit->totalAmount = value;
        unit->totalAmountLastFrame = value;
        unit->lifetimeMaxAmount = value;
        break;
    default:
        unit->totalAmount = value;
        unit->totalAmountLastFrame = value;
        unit->lifetimeMaxAmount = value;
        unit->availableAmount = value;
        break;
    }
}
float CombatFunctions::attack(Unit* unit, int amount)
{
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, 99};
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

void CombatFunctions::takeDamage(Unit* unit, float damage)
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

void CombatFunctions::use(int amount, Unit* unit, SiegeEngine* siege)
{
    siege->useableAmount += amount;
    unit->totalAmount -= amount * siege->cost;
    unit->availableAmount -= amount * siege->cost;
}

void CombatFunctions::reset(Unit* unit, int amount)
{
    for (int i = 0; i < amount && unit->availableAmount < unit->totalAmount; i++)
    {
        unit->availableAmount += i;
    }
}
} // gl3