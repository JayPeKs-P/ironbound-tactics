//
// Created by Julian Kalb on 19/07/2025.
//

#pragma once
#include <random>
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"


namespace gl3 {

class CombatFunctions {
public:
    static void setAmount(Unit* unit,  int value);
    static float attack(Unit* unit, int amount);
    static void takeDamage(Unit* unit, float damage);
    static void UseSiegeEngine(int iAmountTarget, Unit* pUnitActor, SiegeEngine* pSiegeEngineTarget);
    static void ResetUnit(Unit* unit, int amount);
};

} // gl3