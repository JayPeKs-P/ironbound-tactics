//
// Created by Julian Kalb on 05/12/2024.
//

#include "Army.h"

#include <random>

using namespace gl3;

Army::Army(int numInfantry, int numArcher, int numSiege)
{
    // createTroups(UnitHumanoid::Type::Infantry, "gltf/planet.glb", numInfantry);
    // createTroups(UnitHumanoid::Type::Archer, "gltf/enemy.glb", numArcher);
    //createTroups(UnitHumanoid::Type::Siege, "gltf/spaceship.glb", numSiege);
    this->armySize = maxArmySize;
}

//
// /*The way this function is implemented will probably always remove units in a set order,
// depending on the order they were pushed back on creation*/
//
// float Army::takeDamage(float totalDamage)
// {
//     for (auto it = this->units.begin(); it != this->units.end();)
//     {
//         if (!(*it)->isReady())
//         {
//             ++it;
//             continue;
//         }
//
//         float damage = std::min(totalDamage, (*it)->getLifePoints());
//         (*it)->takeDamage(damage);
//         totalDamage -= damage;
//
//         if ((*it)->getLifePoints() <= 0)
//         {
//             // this->armySize -= (*it)->getCommandPoints();
//             it = this->units.erase(it);
//         }
//         else
//         {
//             ++it;
//         }
//
//         if (totalDamage <= 0)
//             break;
//     }
//     return this->armySize;
// }
//
//
// float Army::dealDamage() const
// {
//     float totalDamage = 0;
//     for (auto& unit : this->units)
//     {
//         if (unit->isReady())
//         {
//             // totalDamage += 0.3f * unit->getCommandPoints();
//         }
//         else
//         {
//             unit->setReady(true);
//         }
//     }
//     return totalDamage;
// }
//
// void Army::setDefending(Unit unit, int num)
// {
//     int count = 0;
//     for (auto& unit : this->units)
//     {
//         // if (num <= count) { break; }
//         // if (unit->getType() == unitType && unit->isReady())
//         // {
//         //     unit->setReady(false);
//         //     count++;
//         // }
//     }
// }
//
// float Army::getCommandPoints(Unit unit, int num) const
// {
//     float totalCommandPoints = 0;
//     int count = 0;
//     for (auto& unit : this->units)
//     {
//         // if (num <= count) { break; }
//         // if (unit->getType() == unitType)
//         // {
//         //     totalCommandPoints += unit->getCommandPoints();
//         //     count++;
//         // }
//     }
//     return totalCommandPoints;
// }
//
// void Army::createTroups(Unit unit,
//                         int amount)
// {
//     for (int i = 0; i < amount; i++)
//     {
//         // auto unit = std::make_unique<Unit>(
//         //     //type,
//         //                                    gltfAssetPath,
//         //                                    unitRandomPosition,
//         //                                    0.0f,
//         //                                    glm::vec3(0.05f, 0.05f, 0.0f),
//         //                                    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
//         // maxArmySize += unit->getCommandPoints();
//         // units.push_back(std::move(unit));
//     }
// }
