//
// Created by julia on 12/12/2024.
//

#pragma once
#include "Entity.h"

namespace gl3 {

class UnitSiege: public Entity {
    public:
    enum class Type
    {
        Catapult,
        Assault_Cover
    };
    UnitSiege(Type type, const std::filesystem::path &gltfAssetPath,
        glm::vec3 position, float zRotation,
        glm::vec3 scale, glm::vec4 color);
    private:
    Type type;
    float commandPoints;
    float lifePoints;
    int speed = 1;
    bool canDefend = false;
    bool ready = true;
};

} // gl3