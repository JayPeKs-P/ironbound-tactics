//
// Created by julia on 05/12/2024.
//

#pragma once

#include <vector>

#include "../entities/Unit.h"

namespace gl3 {

class Army {
    public:
    float maxArmySize = 0;
    Army(int amountOfInfantry, int amountOfArcher, int amountOfSiege, glm::vec3 center);
    float takeDamage(float totalDamage);
    float dealDamage() const;
    void setDefending(Unit::Type unitType, int num);
    float getCommandPoints(Unit::Type unitType, int num) const;
    [[nodiscard]] float getArmySize() const
    {
        return armySize;
    }

    private:
    float armySize = 0;
    glm::vec3 center;
    std::vector<std::unique_ptr<Unit>>  units;
    void createTroups(Unit::Type type,
        const std::filesystem::path &gltfAssetPath,
        int amount);

};

} // gl3