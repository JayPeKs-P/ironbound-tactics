//
// Created by Julian Kalb on 20/06/2025.
//

#pragma once
namespace gl3::engine { class Game; }
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
        friend class gl3::engine::Game;
    public:

        explicit System(Game &engine) : engine(engine) {}

        // /// Prevent move semantics.
        // System(System &&) = delete;
        //
        // /// Delete copy constructor.
        // System(const System &) = delete;
        // virtual ~System() = default;

    protected:
        /// @brief Create a System object and store a reference to the game instance.
        ///
        /// @param engine Reference to the core game object.

        engine::Game &engine;
    };
}