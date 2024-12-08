//
// Created by julia on 05/12/2024.
//

#include "Unit.h"

using namespace gl3;

Unit::Unit(Type type, const std::filesystem::path &gltfAssetPath):
Entity(Shader("shaders/shaded/vertexShader.vert",
    "shaders/shaded/fragmentShader.frag"),
    Mesh(gltfAssetPath)
    ),
type(type)
{
    if (type == Type::Infantry)
    {
        this->commandPoints = 5;
        this->lifePoints = this->commandPoints;
        this->isSiege = false;
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
        this->isSiege = true;
        this->canDefend = true;
    }
}

int Unit::takeDamage(int damage)
{
    this->lifePoints -= damage;
    return this->lifePoints;
}
