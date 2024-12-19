//
// Created by julia on 05/12/2024.
//

#pragma once
#include "../Unit.h"

namespace gl3 {

class UnitHumanoid: public Unit {
    public:
    enum class Type{Infantry,
        Archer
    };
    UnitHumanoid(Type type, const std::filesystem::path &gltfAssetPath,
        glm::vec3 position, float zRotation,
        glm::vec3 scale, glm::vec4 color);

    [[nodiscard]] Type getType() const
    {
        return type;
    }

private:
    Type type;
    PlaceInOrder place;


};

} // gl3