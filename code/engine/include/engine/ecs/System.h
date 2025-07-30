//
// Created by Julian Kalb on 20/06/2025.
//

#pragma once
#include "engine/Game.h"
/// @defgroup CoreSystems Engine Systems
/// All ECS systems that handle logic or rendering
/// @ingroup Engine
namespace gl3::engine::ecs {
    /// @ingroup CoreSystems
    /// @brief Abstract base class for all ECS systems.
    ///
    /// A System has access to the ECS context (engine::Game object).
    /// All custom systems should inherit from this class.
    class System {
    public:

        /// @brief Create a System object and store a reference to the game instance.
        ///
        /// @param engine Reference to the core game object.
        explicit System(Game &engine) : engine(engine) {}

        /// Prevent move semantics.
        System(System &&) = delete;

        /// Delete copy constructor.
        System(const System &) = delete;
        virtual ~System() = default;

    protected:
        engine::Game &engine;

        /// @brief Function to check, if one or more entities have the specified component.
        ///
        /// This utility function returns true, if all entities, whose IDs are provided, have a Component of type T.
        /// @tparam  T Component type to check.
        /// @tparam Guids List of guid_t values of the entities to check.
        /// @return true if all provided entities have component T, false otherwise.
        template <typename T, typename... Guids>
        bool checkIfEntityHasComponent(Guids... guids)
        {
            return (... && engine.componentManager.hasComponent<T>((guids)));
        }
    };
}