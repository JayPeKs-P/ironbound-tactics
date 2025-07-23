//
// Created by Julian Kalb on 22/07/2025.
//

#pragma once
#include "engine/ecs/Gui.h"
#include "../components/CombatSelection.h"
#include "GuiCombat.h"
#include "engine/Events.h"

struct Unit;
struct SiegeEngine;
using gl3::engine::ecs::Gui;

class GuiUnitSelection: public Gui {
    using event_t = gl3::engine::events::Event<GuiUnitSelection, int, int, int>;
public:
    static event_t onAccept;
    GuiUnitSelection(gl3::engine::Game &game, nk_context *ctx, nk_uint& textureID);

    int amountToSpare;
private:
    void render() override;
    void triggerEvent() override;

    void drawSelectionFrame();
    void getComponents(gl3::engine::Game& game);
    void initUnits(gl3::engine::Game& game);

    CombatSelection<GuiCombat>* combatSelection_C = nullptr;

    Unit* pInfU_C = nullptr;
    Unit* pArcU_C = nullptr;
    Unit* pCatU_C = nullptr;
    SiegeEngine* pCatSE_C = nullptr;

    Unit* eInfU_C = nullptr;
    Unit* eArcU_C = nullptr;
    Unit* eCatU_C = nullptr;
    SiegeEngine* eCatSE_C = nullptr;

    GLuint pInfTexID = 0;
    GLuint pArcTexID = 0;
    GLuint pCatTexID = 0;

    nk_color highlightColor = nk_rgba(255, 250, 200, 255);
    nk_color playerColor = nk_rgba(100, 255, 100, 255);
    nk_color enemyColor = nk_rgba(255, 100, 100, 255);
    nk_color numberColor = nk_rgba(100, 200, 255, 255);

    int maxCP = 50;

    int valueSlider1 = 0;
    int valueSlider2 = 0;
    int valueSlider3 = 0;

};
