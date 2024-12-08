//
// Created by julia on 05/12/2024.
//

#include "Army.h"

using namespace gl3;
Army::Army(int numInfantry, int numArcher, int numSiege)
{
    createTroups(Unit::Type::Infantry, "gltf/planet.glb", numInfantry);
    createTroups(Unit::Type::Archer, "gltf/enemy.glb", numArcher);
    createTroups(Unit::Type::Siege, "gltf/spaceship.glb", numSiege);
    armySize = maxArmySize;
}
/*The way this function is implemented will probably always remove units in a set order,
depending on the order they were pushed back on creation*/

int Army::takeDamage(int totalDamage)
{
    for (auto it = units.begin(); it != units.end();)
    {
        if (!(*it)->isReady())
        {
            ++it;
            continue;
        }
        if ((*it)->takeDamage(totalDamage) <= 0)
        {
            armySize-= (*it)->getCommandPoints();
            totalDamage -= (*it)->getCommandPoints();
            it = units.erase(it);
        }
        else
        {
            armySize-=(*it)->getLifePoints();
            ++it;
        }
    }
    return armySize;
}

int Army::dealDamage() const
{
    int totalDamage = 0;
    for (auto& unit: units)
    {
        if (unit->isReady())
        {
            totalDamage += unit->getCommandPoints();
        }
        else
        {
            unit->setReady(true);
        }
    }
    return totalDamage;
}

void Army::setDefending(Unit::Type unitType, int num)
{
    int count = 0;
    for (auto& unit: units)
    {
        if (num <= count){break;}
        if (unit->getType() == unitType && unit->isReady())
        {
            unit->setReady(false);
            count ++;
        }
    }
}

int Army::getCommandPoints(Unit::Type unitType, int num) const
{
    int totalCommandPoints = 0;
    int count = 0;
    for (auto& unit: units)
    {
        if (num <= count){break;}
        if (unit->getType() == unitType)
        {
            totalCommandPoints += unit->getCommandPoints();
            count++;
        }
    }
    return totalCommandPoints;
}

void Army::createTroups(Unit::Type type,
                        const std::filesystem::path &gltfAssetPath,
                        int amount)
{
    for (int i = 0; i < amount; i++)
    {
        auto unit = std::make_unique<Unit>(type,gltfAssetPath);
        maxArmySize += unit->getCommandPoints();
        units.push_back(std::move(unit));
    }
}
