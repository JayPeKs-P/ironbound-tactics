//
// Created by julia on 25/02/2025.
//

#pragma once
#include "nuklear.h"
#include "../entities/unitTypes/Archer.h"
#include "../entities/unitTypes/Infantry.h"

namespace gui {

class GuiCombat {
public:
    GuiCombat(struct nk_context *ctx, nk_uint textureID);
    ~GuiCombat() = default;

    void GuiCombat::drawRender(int windowWidth, int windowHeight);

private:
    gl3::Infantry pInf;
    gl3::Archer pArch;
    gl3::Infantry eInf;
    gl3::Archer eArch;

    nk_uint textureID;

    struct nk_image GuiCombat::getTileImage(
    int tileX,
    int tileY,
    int tileWidth,
    int tileHeight,
    int atlasWidth,
    int atlasHeight);

    // struct nk_image atlas;

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

    nk_context* ctx;


};

} // gl3