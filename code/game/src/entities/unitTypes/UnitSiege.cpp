
#include "UnitSiege.h"

using namespace gl3;

UnitSiege::UnitSiege(Type type,
    const std::filesystem::path& gltfAssetPath,
    glm::vec3 position,
    float zRotation,
    glm::vec3 scale,
    glm::vec4 color):
Unit(gltfAssetPath, position, zRotation, scale, color),
type(type)
{
    if (type == Type::Catapult)
    {
        this->commandPoints = 45;
        this->lifePoints = this->commandPoints;
        this->canDefend = false;
        this->place = NextTurn;
    }
    else if (type == Type::Assault_Cover)
    {
        this->commandPoints = 55;
        this->lifePoints = this->commandPoints;
        this->canDefend = true;
        this->place = First;
    }
}
