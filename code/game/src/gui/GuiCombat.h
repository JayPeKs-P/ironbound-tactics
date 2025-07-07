//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include <optional>
#include "engine/ecs/Gui.h"
#include "../components/TagComponent.h"
#include "../components/CombatSelection.h"

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
    std::optional<Category> selectedOne;
    std::optional<Category> selectedTwo;

    int infAmount = 40;
    int archAmount = 30;
    int siegeAmount = 10;

    int amountOfTroups = 0;
    int valueAttack = 0;
    int valueDefend = 0;
    int valueCatapult = 0;
    int valueAssaultCover = 0;
    int valueDefendInf = 0;
    //---

    nk_size healthInfantryPlayer = 100;
    nk_size healthArcherPlayer = 100;
    nk_size healthSiegePlayer = 100;
    nk_size healthInfantryAI = 100;
    nk_size healthArcherAI = 100;
    nk_size healthSiegeAI = 100;


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



};
