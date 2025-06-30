//
// Created by julia on 24/06/2025.
//

#pragma once

#include "../Unit.h"
#include "../unitTypes/Infantry.h"

namespace gl3 {
class Catapult: public Unit {

    public:
        Catapult();
        ~Catapult() = default;
    [[nodiscard]] bool getIsOccupied(){return isOccupied;}
    void occupy(Infantry *unit);
    void abandon();

    protected:

    private:

    bool isOccupied = false;
    Unit* occupier = nullptr;

};

} // gl3