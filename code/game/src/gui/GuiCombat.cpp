//
// Created by Julian Kalb on 25/02/2025.
//

#include "GuiCombat.h"
#include "engine/util/Debug.h"
#include <iostream>

#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"
#include "../components/TagComponent.h"

using namespace gl3;

std::string getType(Unit &unit)
{
    switch (unit.category)
    {
    case UnitCategory::INFANTRY:
        return "Infantry";
    case UnitCategory::ARCHER:
        return "Archer";
    case UnitCategory::CATAPULT:
        return "Catapult";
    case UnitCategory::ASSAULT_COVER:
        return "Asset Cover";
    default:
        return "Unknown";
    }
}
std::string getOwner(Tag tag)
{
    switch (tag)
    {
    case Tag::PLAYER:
        return "Player";
    case Tag::ENEMY:
        return "Enemy";
    default:
        return "Unknown";
    }
}


GuiCombat::GuiCombat(gl3::engine::Game &game, nk_context* ctx, nk_uint& textureID)
    : Gui(game, ctx,textureID)
{
    engine.onAfterStartup.addListener([&](engine::Game& game)
    {
        getComponents(game);
    });

}

void GuiCombat::renderGUI() {
    if (nk_begin(ctx, "Top Row",
        nk_rect(0, 0,
            windowWidth, windowHeight / 13),
            NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
    {
        drawTopRow();
    }
    nk_end(ctx);

    if (combatSelection_C->selectionOne != nullptr)
    {
        if (combatSelection_C->selectionTwo == nullptr)
        {
            actionBounds = nk_rect(
            windowWidth / 4, windowHeight - windowHeight / 4,
            windowWidth / 2, windowHeight / 4);
        }else
        {
            actionBounds = nk_rect(
            windowWidth / 4, windowHeight - windowHeight / 3,
            windowWidth / 2, windowHeight / 3);
        }
        if (nk_begin(ctx, "Actions",
            actionBounds,
            NK_WINDOW_BORDER | NK_WINDOW_SCROLL_AUTO_HIDE))
        {
            drawActions();
            nk_end(ctx);
        }
    }
}

void GuiCombat::drawTopRow()
{
    float ratio[] = {0.05, 0.01,  0.1, 0.2, 0.3, 0.2, 0.1 };
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight/20, 7, ratio);

    if (nk_button_label(ctx, "esc"))
    {

    }

    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer

    auto currentInf = pInfU_C->totalAmount;
    auto currentArc = pArcU_C->totalAmount;
    auto currentCat = pCatSE_C->useableAmount * pCatSE_C->cost;
    auto currenTotal = currentInf + currentArc + currentCat;
    auto maxAmount = pInfU_C->lifetimeMaxAmount + pArcU_C->lifetimeMaxAmount;
    auto hp = static_cast<unsigned long long>(100.0f * currenTotal/maxAmount);
    nk_label(ctx, "Player", NK_TEXT_LEFT);
    nk_progress(ctx, &hp, 100, NK_FIXED);

    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer

    currentInf = eInfU_C->totalAmount;
    currentArc = eArcU_C->totalAmount;
    currentCat = eCatSE_C->useableAmount * eCatSE_C->cost;
    currenTotal = currentInf + currentArc + currentCat;
    maxAmount = eInfU_C->lifetimeMaxAmount + eArcU_C->lifetimeMaxAmount;
    hp = static_cast<unsigned long long>(100.0f * currenTotal/maxAmount);
    nk_progress(ctx, &hp, 100, NK_FIXED);
    nk_label(ctx, "Enemy", NK_TEXT_RIGHT);
}

void GuiCombat::drawActions()
{
    auto unitOne = combatSelection_C->selectionOne;
    auto tagSelectionOne = engine.componentManager.getComponent<TagComponent>(unitOne->entity()).value;

    auto unitTwo = combatSelection_C->selectionTwo;

    nk_layout_row_dynamic(ctx, windowHeight/20, 4);
    nk_label(ctx, "Type: ", NK_TEXT_LEFT);
    nk_label_colored(ctx, getType(*unitOne).c_str(), NK_TEXT_LEFT, highlightColor);
    nk_label(ctx, "Owner: ", NK_TEXT_LEFT);
    if (tagSelectionOne == Tag::PLAYER)
    {
        nk_label_colored(ctx, getOwner(tagSelectionOne).c_str(), NK_TEXT_LEFT, playerColor);
    }else
    {
        nk_label_colored(ctx, getOwner(tagSelectionOne).c_str(), NK_TEXT_LEFT, enemyColor);
    }

    nk_label(ctx, "Total  Left: ", NK_TEXT_LEFT);
    nk_label_colored(ctx, std::to_string(unitOne->totalAmount).c_str(), NK_TEXT_LEFT, numberColor);
    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer

    nk_layout_row_dynamic(ctx, windowHeight/30, 6);
    nk_label(ctx, "HP: ",NK_TEXT_LEFT);
    nk_label_colored(ctx, std::to_string(static_cast<int>(unitOne->hpValue)).c_str(), NK_TEXT_LEFT, numberColor);
    nk_label(ctx, "Atk: ", NK_TEXT_LEFT);
    nk_label_colored(ctx, std::to_string(static_cast<int>(unitOne->attackValue)).c_str(), NK_TEXT_LEFT, numberColor);
    nk_label(ctx, "Acc: ", NK_TEXT_LEFT);
    nk_label_colored(ctx, std::to_string(unitOne->accuracy).c_str(), NK_TEXT_LEFT, numberColor);

    float ratio[] = {0.15, 0.4, 0.05, 0.1 };
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight/20, 4, ratio);
    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
    if (tagSelectionOne == Tag::PLAYER)
    {
        nk_label(ctx, "Left  this  turn: ", NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(unitOne->availableAmount).c_str(), NK_TEXT_LEFT, playerColor);
    }


    if (unitTwo != nullptr)
    {
        auto tagSelectionTwo = engine.componentManager.getComponent<TagComponent>(unitTwo->entity()).value;
        if (tagSelectionOne == Tag::ENEMY)
        {
            combatSelection_C->selectionOne = unitTwo;
            combatSelection_C->selectionTwo = nullptr;
            return;
        }
        if (tagSelectionTwo == Tag::ENEMY && unitOne->availableAmount != 0)
        {
            nk_layout_row_dynamic(ctx, windowHeight/20, 3);
            if (nk_button_label(ctx, "Attack"))
            {
                combatSelection_C->attack.invoke(unitOne->category, value, unitTwo->category);
                combatSelection_C->selectionOne = nullptr;
                combatSelection_C->selectionTwo = nullptr;
                value = 0;
            }
            int availableTroups = unitOne->availableAmount;
            nk_slider_int(ctx, 0, &value, availableTroups, 1);
            nk_label_colored(ctx, getType(*unitTwo).c_str(), NK_TEXT_CENTERED, enemyColor);
            nk_layout_row_dynamic(ctx, windowHeight/30, 3);

            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
            nk_label(ctx, std::to_string(value).c_str(),NK_TEXT_CENTERED);
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
        }else if (tagSelectionTwo == Tag::PLAYER && unitOne->availableAmount != 0)
        {
            if (!engine.componentManager.hasComponent<SiegeEngine>(unitTwo->entity()))
            {
                combatSelection_C->selectionOne = unitTwo;
                combatSelection_C->selectionTwo = nullptr;
                return;
            }
            auto siege_C = engine.componentManager.getComponent<SiegeEngine>(unitTwo->entity());
            auto cost = siege_C.cost;

            nk_layout_row_dynamic(ctx, windowHeight/20, 3);
            if (nk_button_label(ctx, "Use"))
            {
                combatSelection_C->use.invoke(unitOne->category, value, unitTwo->category);
                combatSelection_C->selectionOne = nullptr;
                combatSelection_C->selectionTwo = nullptr;
                value = 0;
            }

            if(unitTwo->totalAmount*cost <= unitOne->totalAmount)
            {
                auto unusedAmount = unitTwo->totalAmount - siege_C.useableAmount;
                nk_slider_int(ctx, 0, &value, unusedAmount, 1);
            }else
            {
                int canUseAmount = unitOne->availableAmount / cost;
                nk_slider_int(ctx, 0, &value, canUseAmount, 1);
            }
            nk_label_colored(ctx, getType(*unitTwo).c_str(), NK_TEXT_CENTERED, playerColor);

            nk_layout_row_dynamic(ctx, windowHeight/30, 3);
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
            nk_label(ctx, std::to_string(value).c_str(),NK_TEXT_CENTERED);
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer

        }else
        {
            combatSelection_C->selectionOne = unitTwo;
            combatSelection_C->selectionTwo = nullptr;
        }
    }
}

void GuiCombat::getComponents(engine::Game &game)
{
    game.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {
        auto &tag = game.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag == Tag::PLAYER)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                pInfU_C = &unit;
            }else if (unit.category == UnitCategory::ARCHER)
            {
                pArcU_C = &unit;
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                pCatU_C = &unit;
                pCatSE_C = &game.componentManager.getComponent<SiegeEngine>(unit.entity());
            }
        }else if (tag == Tag::ENEMY)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                eInfU_C = &unit;
            }else if (unit.category == UnitCategory::ARCHER)
            {
                eArcU_C = &unit;
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                eCatU_C = &unit;
                eCatSE_C = &game.componentManager.getComponent<SiegeEngine>(unit.entity());
            }
        }
    });
    game.componentManager.forEachComponent<CombatSelection<GuiCombat>>([&] (CombatSelection<GuiCombat> &sel)
    {
        combatSelection_C = &sel;
    });
}
