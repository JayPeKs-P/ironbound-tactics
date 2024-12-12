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
Entity(Shader("shaders/shaded/vertexShader.vert",
    "shaders/shaded/fragmentShader.frag"),
    Mesh(gltfAssetPath),
    position,
    zRotation,
    scale,
    color),
type(type)
{
    if (type == Type::Infantry)
    {
        this->commandPoints = 5;
        this->lifePoints = this->commandPoints;
        this->canDefend = true;
    }
    else if (type == Type::Archer)
    {
        this->commandPoints = 10;
        this->lifePoints = this->commandPoints;
        this->canDefend = true;
    }
    else
    {
        this->commandPoints = 45;
        this->lifePoints = this->commandPoints;
        this->canDefend = true;
    }
}

float UnitHumanoid::takeDamage(float damage)
{
    float actualDamage = std::max(0.0f, std::min(damage, this->lifePoints));
    this->lifePoints -= actualDamage;
    return this->lifePoints;

}

