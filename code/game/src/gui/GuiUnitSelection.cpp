//
// Created by Julian Kalb on 22/07/2025.
//

#include "GuiUnitSelection.h"

#include "../components/Model2D.h"
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"
#include "../components/TagComponent.h"
#include "../logic/CombatFunctions.h"
#include "../systems/CombatController.h"
#include "engine/Texture.h"
#include "engine/util/Debug.h"

using namespace gl3;
GuiUnitSelection::event_t GuiUnitSelection::onAccept;
GuiUnitSelection::GuiUnitSelection(engine::Game& game, nk_context* ctx, nk_uint& textureID):
Gui(game, ctx, textureID),
amountToSpare(50)
{
    onAccept.addListener([&](int a, int b, int c)
    {
        DEBUG_LOG(
            << "TRIGGERED: 'onAccept'"
            );
    });
    getComponents(game);
    pInfTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png", false);
    pArcTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png", false);
    pCatTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png", false);
}

GuiUnitSelection::~GuiUnitSelection()
{
    glDeleteTextures(1, &pInfTexID);
    glDeleteTextures(1, &pArcTexID);
    glDeleteTextures(1, &pCatTexID);
}

void GuiUnitSelection::render()
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

void GuiUnitSelection::invokeSceneChange()
{
    onAccept.invoke(valueSlider1, valueSlider2, valueSlider3);
}

void GuiUnitSelection::drawSelectionFrame()
{

    nk_layout_row_dynamic(ctx, windowHeight / 10, 2);
    nk_label(ctx, "Free  Slots  ", NK_TEXT_RIGHT);
    nk_label_colored(ctx, std::to_string(maxCP).c_str(), NK_TEXT_LEFT, playerColor);

    float ratio[] = {0.01, 0.1, 0.01,  0.2, 0.5, 0.2, 0.1, 0.01};
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight / 13, 7, ratio);

    float frameDuration = 0.1f;
    int totalFrames = 4;
    int currentFrame = static_cast<int>(engine.elapsedTime / frameDuration) % totalFrames;
    auto unitImage = nk_subimage_id(pInfTexID, 192, 192,
        nk_rect(currentFrame * 48, 0, 48, 48));

    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_image(ctx, unitImage);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "Infantry", NK_TEXT_LEFT);
    int canUseAmount1 = maxCP - valueSlider2 - valueSlider3;
    if (canUseAmount1 < valueSlider1) valueSlider1 = canUseAmount1;
    nk_slider_int(ctx, 0, &valueSlider1, canUseAmount1, 1);
    nk_label_colored(ctx, std::to_string(valueSlider1).c_str(), NK_TEXT_CENTERED, numberColor);
    nk_label(ctx, "", NK_TEXT_LEFT);


    unitImage = nk_subimage_id(pArcTexID, 192, 192,
        nk_rect(currentFrame * 48, 0, 48, 48));
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_image(ctx, unitImage);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "Archer", NK_TEXT_LEFT);
    int canUseAmount2 = maxCP - valueSlider1 - valueSlider3;
    if (canUseAmount2 < valueSlider2) valueSlider2 = canUseAmount1;
    nk_slider_int(ctx, 0, &valueSlider2, canUseAmount2, 1);
    nk_label_colored(ctx, std::to_string(valueSlider2).c_str(), NK_TEXT_CENTERED, numberColor);
    nk_label(ctx, "", NK_TEXT_LEFT);


    unitImage = nk_subimage_id(pCatTexID, 192, 192,
        nk_rect(currentFrame * 48, 0, 48, 48));
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_image(ctx, unitImage);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "Catapult", NK_TEXT_LEFT);
    int canUseAmount3 = maxCP - valueSlider1 - valueSlider2;
    if (canUseAmount3 < valueSlider3) valueSlider3 = canUseAmount1;
    nk_slider_int(ctx, 0, &valueSlider3, canUseAmount3, pCatSE_C->cost);
    nk_label_colored(ctx, std::to_string(valueSlider3).c_str(), NK_TEXT_CENTERED, numberColor);
    nk_label(ctx, "", NK_TEXT_LEFT);


    nk_layout_row_dynamic(ctx, windowHeight / 20, 3);
    nk_label(ctx, "", NK_TEXT_LEFT);
    if (nk_button_label(ctx, "Accept")) endScene = true;
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

