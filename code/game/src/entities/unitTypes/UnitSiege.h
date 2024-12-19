
#pragma once
#include "../Unit.h"

namespace gl3 {

    class UnitSiege: public Unit {
    public:

        enum class Type
        {
            Catapult,
            Assault_Cover
        };
        [[nodiscard]] Type getType() const
        {
            return type;
        }
        UnitSiege(Type type, const std::filesystem::path &gltfAssetPath,
            glm::vec3 position, float zRotation,
            glm::vec3 scale, glm::vec4 color);

    private:
        Type type;
        PlaceInOrder place;
    };

} // gl3