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
    int combatAction();
    int takeDamage(int damage);

    private:
    Type type;
    int commandPoints;
    bool canDefend = false;
    bool usable = false;

};

} // gl3