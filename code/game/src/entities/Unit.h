//
// Created by julia on 12/12/2024.
//

#pragma once
#include "Entity.h"

namespace gl3 {

class Unit: public Entity {
    public:
    Unit(const std::filesystem::path &gltfAssetPath,
        glm::vec3 position, float zRotation,
        glm::vec3 scale, glm::vec4 color);
    float takeDamage(float damage);

    [[nodiscard]] float getCommandPoints() const
    {
        return commandPoints;
    }

    [[nodiscard]] bool isReady() const
    {
        return ready;
    }

    [[nodiscard]] float getLifePoints() const
    {
        return lifePoints;
    }

    void setReady(bool state)
    {
        ready = state;
    }

private:
    float commandPoints;
    float lifePoints;
    bool canDefend = false;
    bool ready = true;

};

} // gl3