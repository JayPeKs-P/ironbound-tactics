//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/Gui.h"
#include "../components/CombatSelection.h"
struct Unit;
struct SiegeEngine;
using gl3::engine::ecs::Gui;

class GuiCombat: public Gui {
    using event_t = gl3::engine::events::Event<GuiCombat>;
public:
    static event_t startRound;
    static event_t startEndOfTurn;
    GuiCombat(gl3::engine::Game &game, nk_context *ctx, nk_uint& textureID);


private:
    void render() override;
    void invokeSceneChange() override;
    void drawStartRoundWindow();
    void drawTopRow();
    void drawEndTurnWindow();
    void drawActions();
    void getComponents(gl3::engine::Game& game);

    CombatSelection<GuiCombat>* combatSelection_C = nullptr;

    Unit* pInfU_C = nullptr;
    Unit* pArcU_C = nullptr;
    Unit* pCatU_C = nullptr;
    SiegeEngine* pCatSE_C = nullptr;

    Unit* eInfU_C = nullptr;
    Unit* eArcU_C = nullptr;
    Unit* eCatU_C = nullptr;
    SiegeEngine* eCatSE_C = nullptr;

    struct nk_rect actionBounds;
    nk_color highlightColor = nk_rgba(255, 250, 200, 255);
    nk_color playerColor = nk_rgba(100, 255, 100, 255);
    nk_color enemyColor = nk_rgba(255, 100, 100, 255);
    nk_color numberColor = nk_rgba(100, 200, 255, 255);

    int value = 0;

    bool justEndedTurn = false;
    bool justStarted = true;
    int currentRound = 1;
    int currentTurn = 1;
    float countdownStartRound = 3.0f;



};
