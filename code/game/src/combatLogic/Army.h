//
// Created by julia on 05/12/2024.
//

#pragma once

#include <vector>
#include "../entities/Entity.h"

#include "../entities/unitTypes//UnitHumanoid.h"

namespace gl3
{
    class Army : Entity
    {
    public:
        float maxArmySize = 0;
        Army(int amountOfInfantry, int amountOfArcher, int amountOfSiege, glm::vec3 center);
        void draw(Game* game) override;
        float takeDamage(float totalDamage);
        float dealDamage() const;
        void setDefending(UnitHumanoid::Type unitType, int num);
        float getCommandPoints(UnitHumanoid::Type unitType, int num) const;

        [[nodiscard]] float getArmySize() const
        {
            return armySize;
        }

    private:
        float armySize = 0;
        glm::vec3 center;
        std::vector<std::unique_ptr<UnitHumanoid>> units;
        void createTroups(UnitHumanoid::Type type,
                          const std::filesystem::path& gltfAssetPath,
                          int amount);
    };
} // gl3
