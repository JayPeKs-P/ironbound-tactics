//
// Created by julia on 05/12/2024.
//

#include "UnitHumanoid.h"

using namespace gl3;

UnitHumanoid::UnitHumanoid(Type type,
    const std::filesystem::path &gltfAssetPath,
    glm::vec3 position,
    float zRotation,
    glm::vec3 scale,
    glm::vec4 color):
Unit(gltfAssetPath, position, zRotation, scale, color),
type(type)
{
    if (type == Type::Infantry)
    {
        this->commandPoints = 5;
        this->lifePoints = this->commandPoints;
        this->canDefend = true;
        this->place = Fourth;
    }
    else if (type == Type::Archer)
    {
        this->commandPoints = 10;
        this->lifePoints = this->commandPoints;
        this->canDefend = true;
        this->place = Second;
    }
}

