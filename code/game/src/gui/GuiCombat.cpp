//
// Created by Julian Kalb on 25/02/2025.
//

#include "GuiCombat.h"
#include "engine/util/Debug.h"
#include <cstdio>
#include <iostream>
/* Included to use snprintf() in void drawUnitActions()
 * - needed for now to show cast amountOfTroups to char for nk_label
 */

#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"

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
    nk_style* style = &ctx->style;
    setStyleWindow(style);
    setStyleButton(style);
    setStyleOptionLabel(style);
    setStyleProgressBar(style);
    setStyleSlider(style);
    setStyleText(style);

    engine.onAfterStartup.addListener([&](engine::Game& game)
    {
        getComponents(game);
    });

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
            actionbounds = nk_rect(
            windowWidth / 4, windowHeight - windowHeight / 4,
            windowWidth / 2, windowHeight / 4);
        }else
        {
            actionbounds = nk_rect(
            windowWidth / 4, windowHeight - windowHeight / 3,
            windowWidth / 2, windowHeight / 3);
        }
        if (nk_begin(ctx, "Actions",
            actionbounds,
            NK_WINDOW_BORDER | NK_WINDOW_SCROLL_AUTO_HIDE))
        {
            drawActions();
            nk_end(ctx);
        }
    }
}

void GuiCombat::setStyleWindow(nk_style* style)
{
    // style->window.fixed_background = nk_style_item_image(getTileImage(2, 54, 1, 1, 3072, 3072));
    style->window.fixed_background = nk_style_item_color(nk_rgba(0,0,0,120));
    style->window.border = 10.0f;
    style->window.border_color = nk_rgba(120, 58, 58, 163);
    style->window.header.label_normal = nk_rgba(120, 58, 58, 210);
    style->window.header.label_hover = nk_rgba(120, 58, 58, 210);
    style->window.header.label_active = nk_rgba(120, 58, 58, 210);
    style->window.header.normal = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
    style->window.header.hover = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
    style->window.header.active = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
}

void GuiCombat::setStyleButton(nk_style* style)
{
    style->button.normal = nk_style_item_image(getTileImage(0, 11, 3, 1, 3072, 3072));
    style->button.hover = nk_style_item_image(getTileImage(3, 11, 3, 1, 3072, 3072));
    style->button.active = nk_style_item_image(getTileImage(6, 11, 3, 1, 3072, 3072));
    style->button.text_normal = nk_rgba(255, 255, 255, 255);
    style->button.text_hover = nk_rgba(255, 250, 200, 255);
}

void GuiCombat::setStyleOptionLabel(nk_style* style)
{
    style->option.normal = nk_style_item_image(getTileImage(21, 11, 1, 1, 3072, 3072));
    style->option.hover = nk_style_item_image(getTileImage(22, 11, 1, 1, 3072, 3072));
    style->option.active = nk_style_item_image(getTileImage(23, 11, 1, 1, 3072, 3072));
    style->option.cursor_normal = nk_style_item_image(getTileImage(24, 11, 1, 1, 3072, 3072));
    style->option.cursor_hover = nk_style_item_image(getTileImage(25, 11, 1, 1, 3072, 3072));
    style->option.text_normal = nk_rgba(255, 255, 255, 255);
    style->option.text_hover = nk_rgba(255, 250, 200, 255);
}

void GuiCombat::setStyleProgressBar(nk_style* style)
{
    style->progress.normal = nk_style_item_image(getTileImage(0, 14, 3, 1, 3072, 3072));
    style->progress.cursor_normal = nk_style_item_image(getTileImage(6, 14, 3, 1, 3072, 3072));
}

void GuiCombat::setStyleSlider(nk_style* style)
{
    style->slider.normal = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.hover = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.active = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.cursor_normal = nk_style_item_image(getTileImage(21, 12, 1, 1, 3072, 3072));
    style->slider.cursor_hover = nk_style_item_image(getTileImage(22, 12, 1, 1, 3072, 3072));
    style->slider.cursor_active = nk_style_item_image(getTileImage(23, 12, 1, 1, 3072, 3072));
    style->slider.bar_filled = nk_rgba(100, 100, 200, 200);
    style->slider.bar_normal = nk_rgba(92, 58, 58, 163);
    style->slider.bar_hover = nk_rgba(92, 58, 58, 163);
    style->slider.bar_active = nk_rgba(92, 58, 58, 163);
}

void GuiCombat::setStyleText(nk_style* style)
{
    style->text.color = nk_rgba(255, 255, 255, 255);
}

struct nk_image GuiCombat::getTileImage(int tileX, int tileY, int tileWidth, int tileHeight, int atlasWidth, int atlasHeight)
{
    return nk_subimage_id(textureID, atlasWidth, atlasHeight,
                          nk_rect(tileX * 48, tileY * 48, tileWidth * 48, tileHeight * 48));
}
