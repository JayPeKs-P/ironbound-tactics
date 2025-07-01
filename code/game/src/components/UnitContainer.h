//
// Created by Julian Kalb on 30/06/2025.
//

#pragma once
#include <vector>
#include <memory>

#include "engine/ecs/Component.h"

namespace gl3 {
template<typename UnitType, int N>
class UnitContainer: Component {
  public:
    using unitContainer_t = std::vector<std::unique_ptr<UnitType>>;

    unitContainer_t units;

};

} // gl3