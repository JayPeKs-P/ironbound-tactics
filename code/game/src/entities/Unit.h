//
// Created by julia on 05/12/2024.
//

#pragma once
#include "Entity.h"

namespace gl3 {

class Unit: public Entity {
    public:
    enum class Type{Infantry, Archer, Siege};
    Unit(Type type, const std::filesystem::path &gltfAssetPath);
    int takeDamage(int damage);

    [[nodiscard]] Type getType() const
    {
        return type;
    }

    [[nodiscard]] int getCommandPoints() const
    {
        return commandPoints;
    }

    [[nodiscard]] bool isReady() const
    {
        return ready;
    }

    [[nodiscard]] int getLifePoints() const
    {
        return lifePoints;
    }

    void setReady(bool state)
    {
        ready = state;
    }

private:
    Type type;
    int commandPoints;
    int lifePoints;
    bool canDefend = false;
    bool isSiege = false;
    bool ready = true;

};

} // gl3