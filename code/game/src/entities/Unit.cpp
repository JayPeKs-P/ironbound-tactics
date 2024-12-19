//
// Created by julia on 12/12/2024.
//

#include "Unit.h"

using namespace gl3;

Unit::Unit(const std::filesystem::path& gltfAssetPath, glm::vec3 position, float zRotation, glm::vec3 scale,
glm::vec4 color): Entity(Shader("shaders/shaded/vertexShader.vert",
"shaders/shaded/fragmentShader.frag"),
Mesh(gltfAssetPath),
position,
zRotation,
scale,
color)
{
}

float Unit::takeDamage(float damage)
{
    float actualDamage = std::max(0.0f, std::min(damage, this->lifePoints));
    this->lifePoints -= actualDamage;
    return this->lifePoints;
}
