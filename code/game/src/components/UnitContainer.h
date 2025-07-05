//
// Created by Julian Kalb on 30/06/2025.
//

#pragma once
#include <vector>
#include <memory>

#include "engine/ecs/Component.h"
#include "../entities/Unit.h"

namespace gl3 {
    using engine::ecs::Component;
    using engine::ecs::ComponentManager;
    using engine::ecs::guid_t;
    using engine::ecs::Entity;

template<typename UnitType>
class UnitContainer final : public Component {
  public:
    static_assert(std::is_base_of<Unit, UnitType>::value,
        "UnitContainer can only be instantiated with types derived from Unit");
    friend ComponentManager;
    friend Entity;

    std::vector<std::unique_ptr<Unit>> units;

    void add(int n)
    {
        for (int i = 0; i < n; i++)
        {
            auto unit = std::make_unique<UnitType>();
            units.push_back(std::move(unit));
        }
    }
    private:
        explicit UnitContainer(guid_t owner) : Component(owner){ }

};

} // gl3