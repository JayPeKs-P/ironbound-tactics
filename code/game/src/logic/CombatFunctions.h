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
    static void use(int amount, Unit* unit, SiegeEngine* siege);
    static void reset(Unit* unit, int amount);
};

} // gl3