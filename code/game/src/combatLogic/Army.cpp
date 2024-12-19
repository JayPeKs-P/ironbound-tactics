//
// Created by julia on 05/12/2024.
//

#include "Army.h"

#include <random>

using namespace gl3;

Army::Army(int numInfantry, int numArcher, int numSiege, glm::vec3 center):
    Entity(Shader("shaders/shaded/vertexShader.vert",
                  "shaders/shaded/fragmentShader.frag"),
           Mesh("gltf/planet.glb"),
           center,
           0.0f,
           glm::vec3(1.0f, 1.0f, 0.0f),
           glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)),
    center(center)
{
    createTroups(UnitHumanoid::Type::Infantry, "gltf/planet.glb", numInfantry);
    createTroups(UnitHumanoid::Type::Archer, "gltf/enemy.glb", numArcher);
    //createTroups(UnitHumanoid::Type::Siege, "gltf/spaceship.glb", numSiege);
    this->armySize = maxArmySize;
}

void Army::draw(Game* game)
{
    for (auto& unit : units)
    {
        unit->draw(game);
    }
}

/*The way this function is implemented will probably always remove units in a set order,
depending on the order they were pushed back on creation*/

float Army::takeDamage(float totalDamage)
{
    for (auto it = this->units.begin(); it != this->units.end();)
    {
        if (!(*it)->isReady())
        {
            ++it;
            continue;
        }

        float damage = std::min(totalDamage, (*it)->getLifePoints());
        (*it)->takeDamage(damage);
        totalDamage -= damage;

        if ((*it)->getLifePoints() <= 0)
        {
            this->armySize -= (*it)->getCommandPoints();
            it = this->units.erase(it);
        }
        else
        {
            ++it;
        }

        if (totalDamage <= 0)
            break;
    }
    return this->armySize;
}


float Army::dealDamage() const
{
    float totalDamage = 0;
    for (auto& unit : this->units)
    {
        if (unit->isReady())
        {
            totalDamage += 0.3f * unit->getCommandPoints();
        }
        else
        {
            unit->setReady(true);
        }
    }
    return totalDamage;
}

void Army::setDefending(UnitHumanoid::Type unitType, int num)
{
    int count = 0;
    for (auto& unit : this->units)
    {
        if (num <= count) { break; }
        if (unit->getType() == unitType && unit->isReady())
        {
            unit->setReady(false);
            count++;
        }
    }
}

float Army::getCommandPoints(UnitHumanoid::Type unitType, int num) const
{
    float totalCommandPoints = 0;
    int count = 0;
    for (auto& unit : this->units)
    {
        if (num <= count) { break; }
        if (unit->getType() == unitType)
        {
            totalCommandPoints += unit->getCommandPoints();
            count++;
        }
    }
    return totalCommandPoints;
}

void Army::createTroups(UnitHumanoid::Type type,
                        const std::filesystem::path& gltfAssetPath,
                        int amount)
{
    std::mt19937 randomNumberEngine{
        //Mersenne Twister generates 32-bit unsigned integers
        std::random_device{}()
    }; // Seed our Mersenne Twister using with a random number from the OS
    std::uniform_real_distribution positionDist{-0.5f, 0.5f};
    for (int i = 0; i < amount; i++)
    {
        glm::vec3 unitRandomPosition = glm::vec3(positionDist(randomNumberEngine),
                                                 positionDist(randomNumberEngine), 0) + center;
        auto unit = std::make_unique<UnitHumanoid>(type,
                                           gltfAssetPath,
                                           unitRandomPosition,
                                           0.0f,
                                           glm::vec3(0.05f, 0.05f, 0.0f),
                                           glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        maxArmySize += unit->getCommandPoints();
        units.push_back(std::move(unit));
    }
}
