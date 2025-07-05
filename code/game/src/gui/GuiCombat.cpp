//
// Created by Julian Kalb on 25/02/2025.
//

#include "GuiCombat.h"

#include <cstdio>
#include <iostream>
#include <ostream>
/* Included to use snprintf() in void drawUnitActions()
 * - needed for now to show cast amountOfTroups to char for nk_label
 */

using namespace gl3;
//TODO add events to this class and let the functions in CombatController subscribe to those

GuiCombat::GuiCombat(guid_t owner, nk_context* ctx, nk_uint& textureID)
    : Component(owner), ctx(ctx), textureID(textureID)
{
    nk_style* style = &ctx->style;
    setStyleWindow(style);
    setStyleButton(style);
    setStyleOptionLabel(style);
    setStyleProgressBar(style);
    setStyleSlider(style);
    setStyleText(style);

}

void GuiCombat::drawPlayerHealthBars(int windowWidth, int windowHeight)
{
        nk_layout_row_dynamic(ctx, 24, 3);
        nk_label(ctx, "Infantry", NK_TEXT_LEFT);
        nk_label(ctx, "Archer", NK_TEXT_LEFT);
        nk_label(ctx, "Siege", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 36, 3);
        nk_progress(ctx, &healthInfantryPlayer, 100, NK_FIXED);
        nk_progress(ctx, &healthArcherPlayer, 100, NK_FIXED);
        nk_progress(ctx, &healthSiegePlayer, 100, NK_FIXED);
}

void GuiCombat::drawEnemyHealthBars(int windowWidth, int windowHeight)
{
        if (nk_begin(ctx, "Enemy",
        nk_rect(windowWidth / 4, 0, windowWidth / 2, windowHeight / 4),
        NK_WINDOW_TITLE|NK_WINDOW_BORDER)) {

        nk_layout_row_dynamic(ctx, 36, 3);
        nk_progress(ctx, &healthInfantryAI, 100, NK_FIXED);
        nk_progress(ctx, &healthArcherAI, 100, NK_FIXED);
        nk_progress(ctx, &healthSiegeAI, 100, NK_FIXED);

        nk_layout_row_dynamic(ctx, 36, 3);
        const char* enemyNames[] = { "Infantry", "Archer", "Siege" };
        Category enemyCategories[] = { Category::Infantry, Category::Archer, Category::Siege };

        for (int i = 0; i < 3; i++) {
            if (nk_option_label(ctx,
            enemyNames[i],
                    owner == OwnerOfUnit::AI &&
                    selectedTwo == enemyCategories[i]) &&
                selectedOne != Category::Empty)
                {
                if (owner != OwnerOfUnit::AI) owner = OwnerOfUnit::AI;
                if (selectedTwo != enemyCategories[i]) selectedTwo = enemyCategories[i];
            }
        }
    }
    nk_end(ctx);
}

void GuiCombat::drawUnitSelectionMenu(int windowWidth, int windowHeight)
{
        if (nk_begin(ctx, "Units",
        nk_rect(windowWidth / 4, windowHeight - windowHeight / 2, windowWidth / 2, 60), NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {

        nk_layout_row_dynamic(ctx, 36, 3);

        const char* unitNames[] = { "Infantry", "Archer", "Siege" };
        Category unitCategories[] = { Category::Infantry, Category::Archer, Category::Siege };
        int unitTroops[] = { infAmount, archAmount, siegeAmount };

        for (int i = 0; i < 3; i++) {
            if (nk_button_label(ctx, unitNames[i])) {
                if (selectedOne == Category::Empty) {
                    selectedOne = unitCategories[i];
                    amountOfTroups = unitTroops[i];
                } else if (selectedOne != unitCategories[i]) {
                    selectedTwo = unitCategories[i];
                    owner = OwnerOfUnit::Player;
                } else {
                    selectedOne = Category::Empty;
                    selectedTwo = Category::Empty;
                    owner = OwnerOfUnit::No_Selection;
                }
            }
        }
    }
    nk_end(ctx);
}

void GuiCombat::drawUnitActions(int selectedUnit)
{
    if (selectedUnit == Category::Empty) return;

    nk_layout_row_dynamic(ctx, 26, 3);
    nk_label(ctx, (selectedUnit == Category::Infantry) ? "Infantry" :
                   (selectedUnit == Category::Archer) ? "Archer" : "Siege", NK_TEXT_LEFT);
    nk_label(ctx, "Available  Units: ", NK_TEXT_LEFT);

    char amountLabel[16];
    snprintf(amountLabel, sizeof(amountLabel), "%d", amountOfTroups);
    nk_label(ctx, amountLabel, NK_TEXT_LEFT);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Available  Actions", NK_MAXIMIZED)) {
        nk_layout_row_dynamic(ctx, 36, 2);

        if (owner == OwnerOfUnit::AI && selectedTwo != Category::Empty) {
            if (nk_button_label(ctx, "Attack"))
            {
                healthInfantryPlayer -= 10.0f;
            }
            nk_slider_int(ctx, 0, &valueAttack, amountOfTroups, 1);
        } else if (selectedTwo == Category::Siege) {
            if (nk_button_label(ctx, "Use Catapult"))
            {
                if (selectedOne == Category::Infantry) amountOfTroups = 20;
            }
            nk_slider_int(ctx, 0, &valueCatapult, amountOfTroups, 1);
            if (nk_button_label(ctx, "Use Assault Cover"))
            {
                if (selectedOne == Category::Infantry) amountOfTroups = 0;
            }
            nk_slider_int(ctx, 0, &valueAssaultCover, amountOfTroups, 1);
        } else if (selectedTwo == Category::Archer) {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == Category::Siege) amountOfTroups = 6;

            }
            nk_slider_int(ctx, 0, &valueDefend, amountOfTroups, 1);
        }else if (selectedTwo == Category::Infantry)
        {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == Category::Siege) amountOfTroups = 0;
            }
            nk_slider_int(ctx, 0, &valueDefendInf, amountOfTroups, 1);
        }
        nk_tree_pop(ctx);
    }
}

void GuiCombat::drawRender(int windowWidth, int windowHeight) {
    drawUnitSelectionMenu(windowWidth, windowHeight);

    if (nk_begin(ctx, "UnitsGroup",
        nk_rect(windowWidth / 4,
                windowHeight - windowHeight / 3,
                windowWidth / 2,
                windowHeight / 3),
            NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
        drawPlayerHealthBars(windowWidth, windowHeight);
        drawUnitActions(selectedOne);
    }
    nk_end(ctx);

    drawEnemyHealthBars(windowWidth, windowHeight);

}

void GuiCombat::resetSelection()
{
    owner = No_Selection;
    selectedOne = Empty;
    selectedTwo = Empty;
}

void GuiCombat::setStyleWindow(nk_style* style)
{
    style->window.fixed_background = nk_style_item_image(getTileImage(2, 54, 1, 1, 3072, 3072));
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
