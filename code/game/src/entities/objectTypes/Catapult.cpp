//
// Created by julia on 24/06/2025.
//

#include "Catapult.h"

namespace gl3 {
    Catapult::Catapult(): Unit(400, 20, 150, 80, 12, CATAPULT)
    {
    }

    void Catapult::occupy(Infantry *unit)
    {
        occupier = unit;
        isOccupied = true;
    }

    void Catapult::abandon()
    {
        occupier = nullptr;
        isOccupied = false;
    }
} // gl3