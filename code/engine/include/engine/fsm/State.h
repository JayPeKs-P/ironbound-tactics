//
// Created by Julian Kalb on 08/08/2025.
//

#pragma once

namespace gl3::engine::fsm {
    class StateMachine;

    /// @brief Abstract base class. Used for Finite State Machine implementation.
    class State {
        friend StateMachine;
        explicit State(const std::string& stateTitle):ID(stateTitle){};
        /// @brief Called by @ref StateMachine when this instance of State is set to current.
        virtual void enter() = 0;
        virtual void update() = 0;
        /// @brief Called by @ref StateMachine when this instance of State is removed as current.
        virtual void exit() = 0;
        std::string ID;
        virtual ~State() = default;
    };
}