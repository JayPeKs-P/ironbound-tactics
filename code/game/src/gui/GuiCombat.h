//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include <optional>
#include "engine/ecs/Gui.h"
#include "../components/TagComponent.h"
#include "../components/CombatSelection.h"
struct Unit;
struct SiegeEngine;
using gl3::engine::ecs::Gui;

class GuiCombat: public Gui {
public:
    GuiCombat(gl3::engine::Game &game, nk_context *ctx, nk_uint& textureID);

    void GuiCombat::renderGUI(int windowWidth, int windowHeight);
    void resetSelection();

private:
    struct nk_image GuiCombat::getTileImage(
    int tileX,
    int tileY,
    int tileWidth,
    int tileHeight,
    int atlasWidth,
    int atlasHeight);


    // TEMPORARILY USED VARIABLES
    //---
    std::optional<gl3::Tag> owner;
    std::optional<UnitCategory> selectedOne;
    std::optional<UnitCategory> selectedTwo;

    int amountOfTroups = 0;
    int value = 0;
    // int value = 0;
    // int value = 0;
    // int value = 0;
    int valueAssaultCover = 0;
    int valueDefendInf = 0;
    //---


    void getComponents(gl3::engine::Game& game);

    void drawPlayerHealthBars(int windowWidth, int windowHeight);
    void drawEnemyHealthBars(int windowWidth, int windowHeight);
    void drawUnitSelectionMenu(int windowWidth, int windowHeight);
    void drawUnitActions();  //selectedOne was an enum class in Game.cpp. might become a normal enum

    void setStyleWindow(nk_style* style);
    void setStyleButton(nk_style* style);
    void setStyleOptionLabel(nk_style* style);
    void setStyleProgressBar(nk_style* style);
    void setStyleSlider(nk_style* style);
    void setStyleText(nk_style* style);

    Unit* pInfU_C = nullptr;
    Unit* pArcU_C = nullptr;
    Unit* pCatU_C = nullptr;
    SiegeEngine* pCatSE_C = nullptr;

    Unit* eInfU_C = nullptr;
    Unit* eArcU_C = nullptr;
    Unit* eCatU_C = nullptr;
    SiegeEngine* eCatSE_C = nullptr;



};
