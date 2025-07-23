//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/Gui.h"
#include "../components/CombatSelection.h"
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

    guid_t pInf_E;
    guid_t pArc_E;
    guid_t pCat_E;

    guid_t eInf_E;
    guid_t eArc_E;
    guid_t eCat_E;

    struct nk_rect actionBounds;
    nk_color highlightColor = nk_rgba(255, 250, 200, 255);
    nk_color playerColor = nk_rgba(100, 255, 100, 255);
    nk_color enemyColor = nk_rgba(255, 100, 100, 255);
    nk_color numberColor = nk_rgba(100, 200, 255, 255);

    int value = 0;

    bool justEndedTurn = false;
    bool justStarted = true;
    int currentRound = 1;
    int currentTurn = 0;
    float countdownStartRound = 3.0f;



};
