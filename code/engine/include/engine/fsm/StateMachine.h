//
// Created by Julian Kalb on 08/08/2025.
//

#pragma once
#include <map>
#include <memory>
#include <string>

#include "State.h"

namespace gl3::engine::fsm {
    class StateMachine {
    public:
        StateMachine();
        ~StateMachine() = default;

        void setState(const std::string& newState);
        void registerState(const std::shared_ptr<State> &state);
        void update();

    protected:
        std::shared_ptr<State> previousState = nullptr;
        std::shared_ptr<State> currentState = nullptr;

        std::map<std::string, std::shared_ptr<State>> stateRegister;
    };
}
