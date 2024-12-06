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
}

void Army::createTroups(Unit::Type type,
    const std::filesystem::path &gltfAssetPath,
    int amount)
{
    for (int i = 0; i < amount; i++)
    {
        auto unit = std::make_unique<Unit>(type,gltfAssetPath);
        units.push_back(std::move(unit));
    }
}
