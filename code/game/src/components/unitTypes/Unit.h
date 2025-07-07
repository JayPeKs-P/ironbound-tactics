//
// Created by Julian Kalb on 12/12/2024.
//

#pragma once

struct Unit{
protected:
    Unit (float hp, float def, float atk, int acc, int critc, int amount)
    :unitLPValue(hp),
    armorValue(def),
    attackValue(atk),
    accuracy(acc),
    critChance(critc),
    totalAmount(amount),
    availableAmount(amount),
    lifetimeMaxAmount(amount){ };
public:
    float unitLPValue;
    float armorValue;
    float attackValue;
    int accuracy;
    int critChance;
    float critMultiplier = 2.0f;
    int lifetimeMaxAmount;
    int totalAmount;
    int availableAmount;

};
