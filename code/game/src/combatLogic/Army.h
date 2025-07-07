//
// Created by Julian Kalb on 05/12/2024.
//

#pragma once

#include <vector>
// #include "../entities/Entity.h"

#include "../entities/Unit.h"

namespace gl3
{
    class Army
    {
    public:
        float maxArmySize = 0;
        Army(int amountOfInfantry, int amountOfArcher, int amountOfSiege);
        float takeDamage(float totalDamage);
        float dealDamage() const;
        void setDefending(Unit unit, int num);
        float getCommandPoints(Unit unit, int num) const;

        [[nodiscard]] float getArmySize() const
        {
            return armySize;
        }


    private:
        float armySize = 0;
        void createTroups(Unit unit,
                          int amount);
    };
} // gl3
