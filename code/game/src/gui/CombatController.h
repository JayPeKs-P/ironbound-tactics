//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/System.h"
#include "engine/Events.h"

#include "../entities/Unit.h"

namespace gl3 {

class CombatController: public engine::ecs::System {
    public:
    // TODO: check if this typedef needs to add <..., CombatController&> like in wp3 engine/game.h
    using eventCC_t = engine::events::Event<CombatController>;
    CombatController(engine::Game &game );
    ~CombatController();

    // events for combat and turn structure
    eventCC_t onStartCombat;
    eventCC_t onBeforeTurn;
    eventCC_t onAfterTurn;
    eventCC_t onEndCombat;

    void handleTurn(bool newRound);

    void init();

    private:

};

} // gui