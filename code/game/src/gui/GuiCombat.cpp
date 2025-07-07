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

}

void GuiCombat::drawPlayerHealthBars(int windowWidth, int windowHeight)
{
    nk_layout_row_dynamic(ctx, windowHeight/30, 3);
    nk_label(ctx, "Infantry", NK_TEXT_LEFT);
    nk_label(ctx, "Archer", NK_TEXT_LEFT);
    nk_label(ctx, "Siege", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, windowHeight/20, 3);
    nk_progress(ctx, &healthInfantryPlayer, 100, NK_FIXED);
    nk_progress(ctx, &healthArcherPlayer, 100, NK_FIXED);
    nk_progress(ctx, &healthSiegePlayer, 100, NK_FIXED);
}

void GuiCombat::drawEnemyHealthBars(int windowWidth, int windowHeight)
{
    if (nk_begin(ctx, "Enemy",
    nk_rect(windowWidth / 4, 0, windowWidth / 2, windowHeight / 4),
    NK_WINDOW_TITLE|NK_WINDOW_BORDER))
    {

        nk_layout_row_dynamic(ctx, windowHeight/20, 3);
        nk_progress(ctx, &healthInfantryAI, 100, NK_FIXED);
        nk_progress(ctx, &healthArcherAI, 100, NK_FIXED);
        nk_progress(ctx, &healthSiegeAI, 100, NK_FIXED);

        nk_layout_row_dynamic(ctx, windowHeight/20, 3);
        const char* enemyNames[] = { "Infantry", "Archer", "Siege" };
        Category enemyCategories[] = { Category::INFANTRY, Category::ARCHER, Category::SIEGE };

        for (int i = 0; i < 3; i++)
        {
            if (nk_option_label(ctx,
            enemyNames[i],
                    owner == Tag::ENEMY &&
                    selectedTwo == enemyCategories[i]) &&
                selectedOne.has_value())
            {
                if (owner != Tag::ENEMY) owner = Tag::ENEMY;
                if (selectedTwo != enemyCategories[i]) selectedTwo = enemyCategories[i];
            }
        }
    }
    nk_end(ctx);
}

void GuiCombat::drawUnitSelectionMenu(int windowWidth, int windowHeight)
{
    if (nk_begin(ctx, "Units",
    nk_rect(windowWidth / 4,
        windowHeight - windowHeight / 2,
        windowWidth / 2, windowHeight/12),
        NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER))
    {

        nk_layout_row_dynamic(ctx, windowHeight/20, 3);

        const char* unitNames[] = { "Infantry", "Archer", "Siege" };
        Category unitCategories[] = { Category::INFANTRY, Category::ARCHER, Category::SIEGE };
        int unitTroops[] = { infAmount, archAmount, siegeAmount };

        for (int i = 0; i < 3; i++)
        {
            if (nk_button_label(ctx, unitNames[i]))
            {
                if (!selectedOne.has_value())
                {
                    selectedOne = unitCategories[i];
                    amountOfTroups = unitTroops[i];
                } else if (selectedOne != unitCategories[i])
                {
                    selectedTwo = unitCategories[i];
                    owner = Tag::PLAYER;
                } else
                {
                    selectedOne.reset();
                    selectedTwo.reset();
                    owner.reset();
                }
            }
        }
    }
    nk_end(ctx);
}

void GuiCombat::drawUnitActions()
{
    if (!selectedOne.has_value()) return;

    nk_layout_row_dynamic(ctx, 26, 3);
    nk_label(ctx, (selectedOne == Category::INFANTRY) ? "Infantry" :
                   (selectedOne == Category::ARCHER) ? "Archer" : "Siege", NK_TEXT_LEFT);
    nk_label(ctx, "Available  Units: ", NK_TEXT_LEFT);

    char amountLabel[16];
    snprintf(amountLabel, sizeof(amountLabel), "%d", amountOfTroups);
    nk_label(ctx, amountLabel, NK_TEXT_LEFT);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Available  Actions", NK_MAXIMIZED))
    {
        nk_layout_row_dynamic(ctx, 36, 2);

        if (owner == Tag::ENEMY && selectedTwo.has_value())
        {
            if (nk_button_label(ctx, "Attack"))
            {
                for (auto& [owner, _] : engine.componentManager.getContainer<CombatSelection<GuiCombat>>())
                {
                    if (engine.componentManager.hasComponent<CombatSelection<GuiCombat>>(owner))
                    {
                        engine.componentManager.getComponent<CombatSelection<GuiCombat>>(owner).attack.invoke(selectedOne.value(), amountOfTroups, selectedTwo.value());
                    }
                }
            }
            nk_slider_int(ctx, 0, &valueAttack, amountOfTroups, 1);
            char amountLabel[16];
            snprintf(amountLabel, sizeof(amountLabel), "%d", valueAttack);
            nk_label(ctx, amountLabel, NK_TEXT_LEFT);
        } else if (selectedTwo == Category::SIEGE)
        {
            if (nk_button_label(ctx, "Use Catapult"))
            {
                if (selectedOne == Category::INFANTRY) amountOfTroups = 20;
            }
            nk_slider_int(ctx, 0, &valueCatapult, amountOfTroups, 1);
            if (nk_button_label(ctx, "Use Assault Cover"))
            {
                if (selectedOne == Category::INFANTRY) amountOfTroups = 0;
            }
            nk_slider_int(ctx, 0, &valueAssaultCover, amountOfTroups, 1);
        } else if (selectedTwo == Category::ARCHER)
        {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == Category::SIEGE) amountOfTroups = 6;

            }
            nk_slider_int(ctx, 0, &valueDefend, amountOfTroups, 1);
        } else if (selectedTwo == Category::INFANTRY)
        {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == Category::SIEGE) amountOfTroups = 0;
            }
            nk_slider_int(ctx, 0, &valueDefendInf, amountOfTroups, 1);
        }
        nk_tree_pop(ctx);
    }
}

void GuiCombat::renderGUI(int windowWidth, int windowHeight) {
    drawUnitSelectionMenu(windowWidth, windowHeight);

    if (nk_begin(ctx, "UnitsGroup",
        nk_rect(windowWidth / 4,
                windowHeight - windowHeight / 3,
                windowWidth / 2,
                windowHeight / 3),
            NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        drawPlayerHealthBars(windowWidth, windowHeight);
        drawUnitActions();
    }
    nk_end(ctx);

    drawEnemyHealthBars(windowWidth, windowHeight);

}

void GuiCombat::resetSelection()
{
    owner.reset();
    selectedOne.reset();
    selectedTwo.reset();
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
