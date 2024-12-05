//
// Created by julia on 05/12/2024.
//

#pragma once
#include "Entity.h"

namespace gl3 {

class Unit: public Entity {
    public:
    Unit(int id);
    int combatAction();
    private:
    int commandPoints;
    bool canDefend = false;
    bool usable = false;

};

} // gl3