//
// Created by Julian Kalb on 12/12/2024.
//

#include "Unit.h"

using namespace gl3;

Unit::Unit(float hp, float def, float atk, int acc, int critc, TypeOfUnit type):
lifePoints(hp),
armorValue(def),
attackValue(atk),
accuracy(acc),
critChance(critc),
typeOfUnit(type)
{

}


float Unit::attack()
{
    int hitRoll = dist(rng);
    if (hitRoll >= accuracy)
    {
        return 0;
    }
    float damage = attackValue;

    int critRoll = dist(rng);
    if (critRoll < critChance)
    {
       damage *= critMultiplier;
    }
    return damage;
}

float Unit::takeDamage(float damage)
{
    float differenceAD = damage - armorValue;
    armorValue -= damage/20;
    if (differenceAD > 0)
    {
       lifePoints -= differenceAD;
    }
    return lifePoints;
}
