//
// Created by julia on 05/12/2024.
//

#pragma once

#include <vector>

#include "../entities/Unit.h"

namespace gl3 {

class Army {
    public:
    int maxArmySize = 0;
    Army(int amountOfInfantry, int amountOfArcher, int amountOfSiege);
    int takeDamage(int totalDamage);
    int dealDamage() const;
    void setDefending(Unit::Type unitType, int num);
    int getCommandPoints(Unit::Type unitType, int num) const;
    private:
    int armySize = 0;
    std::vector<std::unique_ptr<Unit>>  units;
    void createTroups(Unit::Type type,
        const std::filesystem::path &gltfAssetPath,
        int amount);

public:
    [[nodiscard]] int getArmySize() const
    {
        return armySize;
    }
};

} // gl3