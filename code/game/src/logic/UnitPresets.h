//
// Created by Julian Kalb on 23/07/2025.
//
#pragma once
#include "../components/unitTypes/UnitCategory.h"

// PLAYER UNIT PRESETS
struct pInfantry
{
    float hpValue = 100;
    float armorValue = 30;
    float attackValue = 50;
    int accuracyValue = 99;
    int critChance = 10;
    float critMultiplier = 2.0f;
    int lifeTimeMaxAmount = 0;
    int totalAmount = 0;
    int totalAmountLastFrame = 0;
    int availableAmount = 0;
    int speed = 2;
    UnitCategory category = UnitCategory::INFANTRY;
};
struct pArcher
{
    float hpValue = 80;
    float armorValue = 20;
    float attackValue = 40;
    int accuracyValue = 80;
    int critChance = 8;
    float critMultiplier = 2.0f;
    int lifeTimeMaxAmount = 0;
    int totalAmount = 0;
    int totalAmountLastFrame = 0;
    int availableAmount = 0;
    int speed = 1;
    UnitCategory category = UnitCategory::ARCHER;
};
struct pCatapult
{
    float hpValue = 300;
    float armorValue = 20;
    float attackValue = 160;
    int accuracyValue = 80;
    int critChance = 12;
    float critMultiplier = 2.0f;
    int lifeTimeMaxAmount = 0;
    int totalAmount = 0;
    int totalAmountLastFrame = 0;
    int availableAmount = 0;
    int speed = 3;
    UnitCategory category = UnitCategory::CATAPULT;
};

// ENEMY UNIT PRESETS
