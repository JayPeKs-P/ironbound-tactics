//
// Created by Julian Kalb on 22/07/2025.
//

#include "GuiUnitSelection.h"

#include "../components/Model2D.h"
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"
#include "../components/TagComponent.h"
#include "../logic/CombatFunctions.h"
#include "engine/Texture.h"

using namespace gl3;
GuiUnitSelection::GuiUnitSelection(engine::Game& game, nk_context* ctx, nk_uint& textureID):
Gui(game, ctx, textureID)
{
    getComponents(game);
}

void GuiUnitSelection::renderGUI()
{
    if (nk_begin(ctx, "Background",
        nk_rect(0, 0,
            windowWidth, windowHeight),
        NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR))nk_end(ctx);
    if (nk_begin(ctx, "Select  your  troups  Commander",
        nk_rect(windowWidth / 4,  windowHeight / 4,
            windowWidth / 2, windowHeight/ 2),
            NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE))
    {
        drawSelectionFrame();
        nk_end(ctx);
    }
}

void GuiUnitSelection::drawSelectionFrame()
{
    pInfTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png", false);
    pArcTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png", false);
    pCatTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png", false);

    nk_layout_row_dynamic(ctx, windowHeight / 20, 2);
    nk_label(ctx, "Free  Slots  ", NK_TEXT_RIGHT);
    nk_label_colored(ctx, std::to_string(100).c_str(), NK_TEXT_LEFT, playerColor);

    float ratio[] = {0.01, 0.05, 0.01,  0.2, 0.5, 0.2, 0.1, 0.01};
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight / 20, 7, ratio);

    float frameDuration = 0.1f;
    int totalFrames = 4;
    int currentFrame = static_cast<int>(engine.elapsedTime / frameDuration) % totalFrames;
    auto unitImage = nk_subimage_id(pInfTexID, 192, 192,
        nk_rect(currentFrame * 48, 0, 48, 48));

    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_image(ctx, unitImage);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "Infantry", NK_TEXT_LEFT);
    nk_slider_int(ctx, 0, &value, canUseAmount, 1);
}

void GuiUnitSelection::getComponents(engine::Game& game)
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
        }
    });
    game.componentManager.forEachComponent<CombatSelection<GuiCombat>>([&] (CombatSelection<GuiCombat> &sel)
    {
        combatSelection_C = &sel;
    });
}

void GuiUnitSelection::initUnits(engine::Game& game)
{
    game.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {
        auto &tag = game.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag == Tag::PLAYER)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                pInfU_C = &unit;
                CombatFunctions::setAmount(pInfU_C, 20);
            }else if (unit.category == UnitCategory::ARCHER)
            {
                pArcU_C = &unit;
                CombatFunctions::setAmount(pArcU_C, 20);
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                pCatU_C = &unit;
                pCatSE_C = &game.componentManager.getComponent<SiegeEngine>(unit.entity());
                CombatFunctions::setAmount(pCatU_C, 5);
            }
        }
    });
}
