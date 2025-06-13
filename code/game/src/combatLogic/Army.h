//
// Created by Julian Kalb on 05/12/2024.
//

#pragma once

#include <vector>
#include "../entities/Entity.h"

#include "../entities/Unit.h"

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
        void setDefending(Unit unit, int num);
        float getCommandPoints(Unit unit, int num) const;

        [[nodiscard]] float getArmySize() const
        {
            return armySize;
        }


    private:
        float armySize = 0;
        glm::vec3 center;
        std::vector<std::unique_ptr<Unit>> units;
        void createTroups(Unit unit,
                          const std::filesystem::path& gltfAssetPath,
                          int amount);
    };
} // gl3
