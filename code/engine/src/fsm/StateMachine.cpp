//
// Created by Julian Kalb on 08/08/2025.
//

#include "engine/fsm/StateMachine.h"

#include <stdexcept>

namespace gl3::engine::fsm {
    StateMachine::StateMachine()
    {
    }

    void StateMachine::setState(const std::string& newState){
        if (!stateRegister.contains(newState))
        {
            throw std::runtime_error("Failed to change State to" + newState + ". Not an entry in stateRegister");
        }
        previousState->exit();
        previousState = std::move(currentState);
        stateRegister[newState]->enter();
        currentState = stateRegister.at(newState);

    }

    void StateMachine::registerState(const std::shared_ptr<State> &state) {
        if (stateRegister.contains(state->ID))
        {
            throw std::runtime_error("Failed to register " + state->ID +
                " as a new state. There already exists a state with that name.");
        }
        stateRegister[state->ID] = state;
    }
}
