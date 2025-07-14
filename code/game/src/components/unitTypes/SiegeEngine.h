//
// Created by Julian Kalb on 08/07/2025.
//

#pragma once



struct SiegeEngine {
    float hpValue;
    float armorValue;
    float attackValue;
    int accuracy;
    int critChance;
    float critMultiplier = 2.0f;
    int lifetimeMaxAmount;
    int totalAmount;
    int availableAmount;
    int useableAmount = 0;
    int cost;
    int speed;
protected:
    SiegeEngine(float hp, float def, float atk, int acc, int critc, int amount, int cost, int speed)
        : hpValue(hp),
        armorValue(def),
        attackValue(atk),
        accuracy(acc),
        critChance(critc),
        totalAmount(amount),
        availableAmount(amount),
        lifetimeMaxAmount(amount),
        cost(cost),
        speed(speed) { }

};
