//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/Gui.h"
#include "engine/Events.h"
#include "../entities/Unit.h"

using gl3::engine::ecs::Gui;

class GuiCombat: public Gui {
public:
    GuiCombat(gl3::engine::Game &game, nk_context *ctx, nk_uint& textureID);
    //-----Inputevents-----
    using event_t = gl3::engine::events::Event<GuiCombat,
    std::vector<std::unique_ptr<gl3::Unit>>,                //Actor
    int,
    std::vector<std::unique_ptr<gl3::Unit>>>;               //Target
    event_t attack;
    event_t defend;
    event_t use;



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
    enum OwnerOfUnit{No_Selection, Player, AI};
    enum Category {Empty, Infantry, Archer, Siege, Catapult, Assault_Cover};
    OwnerOfUnit owner = OwnerOfUnit::No_Selection;
    Category selectedOne = Category::Empty;
    Category selectedTwo = Category::Empty;

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
    void drawUnitActions(int selectedOne = 0);  //selectedOne was an enum class in Game.cpp. might become a normal enum

    void setStyleWindow(nk_style* style);
    void setStyleButton(nk_style* style);
    void setStyleOptionLabel(nk_style* style);
    void setStyleProgressBar(nk_style* style);
    void setStyleSlider(nk_style* style);
    void setStyleText(nk_style* style);



};
