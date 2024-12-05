//
// Created by julia on 05/12/2024.
//

#pragma once
#include <memory>
#include <vector>

#include "../entities/Unit.h"

namespace gl3 {

class Army {
    public:
    Army(int amountOfInfantry, int amountOfArcher, int amountOfSiege);
    private:
    std::vector<std::unique_ptr<Unit>>  units;
    void createTroups(int amount);


};

} // gl3