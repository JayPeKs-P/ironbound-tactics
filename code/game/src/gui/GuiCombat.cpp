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
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"

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

}

void GuiCombat::drawPlayerHealthBars(int windowWidth, int windowHeight)
{
    auto pInfHP = static_cast<unsigned long long>(100.0 * pInfU_C->totalAmount / pInfU_C->lifetimeMaxAmount);
    auto pArcHP = static_cast<unsigned long long>(100.0 * pArcU_C->totalAmount / pArcU_C->lifetimeMaxAmount);
    auto pCatHP = static_cast<unsigned long long>(100.0 * pCatSE_C->useableAmount / pCatU_C->lifetimeMaxAmount);

    nk_layout_row_dynamic(ctx, windowHeight/30, 3);
    nk_label(ctx, "Infantry", NK_TEXT_LEFT);
    nk_label(ctx, "Archer", NK_TEXT_LEFT);
    nk_label(ctx, "Siege", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, windowHeight/20, 3);
    nk_progress(ctx, &pInfHP, 100, NK_FIXED);
    nk_progress(ctx, &pArcHP, 100, NK_FIXED);
    nk_progress(ctx, &pCatHP, 100, NK_FIXED);
}

void GuiCombat::drawEnemyHealthBars(int windowWidth, int windowHeight)
{
    if (nk_begin(ctx, "Enemy",
    nk_rect(windowWidth / 4, 0, windowWidth / 2, windowHeight / 4),
    NK_WINDOW_TITLE|NK_WINDOW_BORDER))
    {
        auto eInfHP = static_cast<unsigned long long>(100 * eInfU_C->totalAmount / eInfU_C->lifetimeMaxAmount);
        auto eArcHP = static_cast<unsigned long long>(100 * eArcU_C->totalAmount / eArcU_C->lifetimeMaxAmount);
        auto eCatHP = static_cast<unsigned long long>(100 * eCatSE_C->useableAmount / eCatU_C->lifetimeMaxAmount);

        nk_layout_row_dynamic(ctx, windowHeight/20, 3);
        nk_progress(ctx, &eInfHP, 100, NK_FIXED);
        nk_progress(ctx, &eArcHP, 100, NK_FIXED);
        nk_progress(ctx, &eCatHP, 100, NK_FIXED);

        nk_layout_row_dynamic(ctx, windowHeight/20, 3);
        const char* enemyNames[] = { "Infantry", "Archer", "Siege" };
        UnitCategory enemyCategories[] = { UnitCategory::INFANTRY, UnitCategory::ARCHER, UnitCategory::CATAPULT };

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

        const char* unitNames[] = { "Infantry", "Archer", "Catapult" };
        UnitCategory unitCategories[] = { UnitCategory::INFANTRY, UnitCategory::ARCHER, UnitCategory::CATAPULT };
        int unitTroops[] = { pInfU_C->availableAmount, pArcU_C->availableAmount, pCatU_C->availableAmount };

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
    nk_label(ctx, (selectedOne == UnitCategory::INFANTRY) ? "Infantry" :
                   (selectedOne == UnitCategory::ARCHER) ? "Archer" : "Siege", NK_TEXT_LEFT);
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
                        engine.componentManager.getComponent<CombatSelection<GuiCombat>>(owner).attack.invoke(selectedOne.value(), value, selectedTwo.value());
                    }
                }
                resetSelection();
            }
            nk_slider_int(ctx, 0, &value, amountOfTroups, 1);
            char amountLabel[16];
            snprintf(amountLabel, sizeof(amountLabel), "%d", value);
            nk_label(ctx, amountLabel, NK_TEXT_LEFT);
        } else if (selectedTwo == UnitCategory::CATAPULT)
        {
            if (nk_button_label(ctx, "Use Catapult"))
            {
                for (auto& [owner, _] : engine.componentManager.getContainer<CombatSelection<GuiCombat>>())
                {
                    if (engine.componentManager.hasComponent<CombatSelection<GuiCombat>>(owner))
                    {
                        engine.componentManager.getComponent<CombatSelection<GuiCombat>>(owner).use.invoke(selectedOne.value(), value, selectedTwo.value());
                    }
                }
                resetSelection();
            }
            if(pCatU_C->totalAmount*pCatSE_C->cost <= pInfU_C->totalAmount)
            {
                nk_slider_int(ctx, 0, &value, pCatU_C->totalAmount-pCatSE_C->useableAmount, 1);
            }else
            {
                nk_slider_int(ctx, 0, &value, amountOfTroups / pCatSE_C->cost, 1);
            }
            if (nk_button_label(ctx, "(!)Use Assault Cover"))
            {
                //TODO change selected two back to siege from catapult here and in unitSelection()
                for (auto& [owner, _] : engine.componentManager.getContainer<CombatSelection<GuiCombat>>())
                {
                    if (engine.componentManager.hasComponent<CombatSelection<GuiCombat>>(owner))
                    {
                        engine.componentManager.getComponent<CombatSelection<GuiCombat>>(owner).use.invoke(selectedOne.value(), value, selectedTwo.value());
                    }
                }
                resetSelection();
            }
            nk_slider_int(ctx, 0, &valueAssaultCover, amountOfTroups, 1);
        } else if (selectedTwo == UnitCategory::ARCHER)
        {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == UnitCategory::SIEGE) amountOfTroups = 6;

            }
            nk_slider_int(ctx, 0, &value, amountOfTroups, 1);
        } else if (selectedTwo == UnitCategory::INFANTRY)
        {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == UnitCategory::SIEGE) amountOfTroups = 0;
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
    value = 0;
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
