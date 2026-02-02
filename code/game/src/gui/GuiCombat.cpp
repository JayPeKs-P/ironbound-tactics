//
// Created by Julian Kalb on 25/02/2025.
//

#include "GuiCombat.h"
#include "engine/util/Debug.h"
#include <iostream>
#include <ranges>

#include "../components/unitTypes/UnitCategory.h"
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"
#include "../components/TagComponent.h"
#include "../logic/LibCombatFunctions.h"
#include "../systems/CombatController.h"
#include "../systems/GuiHandler.h"
#include "engine/Texture.h"
#include "engine/rendering/Model2D.h"
#include "engine/SoundSystem.h"

using namespace gl3;
GuiCombat::event_t GuiCombat::startRound;
// GuiCombat::event_t GuiCombat::startEndOfTurn;
float GuiCombat::countdownStartRound = 3.0f;

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


GuiCombat::GuiCombat(gl3::engine::Game &game, GuiHandler& guiHandler,  nk_context* ctx, nk_uint& textureID)
    : Gui(game, ctx,textureID),
      m_GuiHandler(guiHandler)
{
    getComponents(game);

    CombatController::turnStart.addListener([&]()
    {

    });
    startRound.addListener([&]()
    {
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_MODE
    DEBUG_LOG(
        << "TRIGGERED EVENT: 'startRound'"
        );
#endif
}
////////////////////////////////////////////////////////////////////////
    });
    auto pSoundSystem = engine::SoundSystem::GetInstance();
    pSoundSystem->PlayMusic(engine::MUSIC_COMBAT_1);
}

GuiCombat::~GuiCombat() {
    for (auto val : m_Textures | std::views::values) {
        glDeleteTextures(1, &val);
    }
    engine.SetSpeedUpValue(1.0f);
}

void GuiCombat::render()
{
    if (CombatController::getCombatState() == CombatState::STARTING_NEW_ROUND)
    {
        drawStartRoundWindow();
    }
    else if (CombatController::getCombatState() == CombatState::MAIN_PHASE)
    {
        if (nk_begin(ctx, "End Turn Window",
            nk_rect(2 * windowWidth / 5, windowHeight / 13,
                windowWidth / 5, windowHeight / 13),
                NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
        {
            drawEndTurnWindow();
        }
        nk_end(ctx);
    }
    else if (CombatController::getCombatState() == CombatState::DEFEAT) {
        DrawDefeatWindow();
    }
    else if (CombatController::getCombatState() == CombatState::REWARD_PHASE) {
        if (DrawRewardWindow()) {
            CombatController::setState(CombatState::PREPARE);
            m_Rewards.clear();
        }
    }


    if (nk_begin(ctx, "Top Row Window",
        nk_rect(0, 0,
            windowWidth, windowHeight / 13),
            NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
    {
        drawTopRow();
    }
    nk_end(ctx);


    auto pCombatSelection = CombatSelection::GetInstance();
    if (pCombatSelection->m_pFirstUnit_C != nullptr)
    {
        auto bounds = nk_rect(
        windowWidth * 4/11, windowHeight - windowHeight / 3.5f,
        windowWidth * 3/11, windowHeight / 3.5f);

        nk_style_item transparancy = nk_style_item_color(nk_color(0, 0, 0, 180));
        nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, transparancy);
        if (nk_begin(ctx, "Info",
            bounds,
            NK_WINDOW_BORDER | NK_WINDOW_SCROLL_AUTO_HIDE))
        {
            DrawFirstSelection();
            nk_end(ctx);
            nk_style_pop_style_item(ctx);
        }
    }
    if (pCombatSelection->m_pSecondUnit_C != nullptr) {
        auto bounds = nk_rect(
        windowWidth * 7/11, windowHeight - windowHeight * 5/21,
        windowWidth * 5/22, windowHeight * 5/21);

        nk_style_item transparancy = nk_style_item_color(nk_color(0, 0, 0, 180));
        nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, transparancy);
        if (nk_begin(ctx, "Actions",
            bounds,
            NK_WINDOW_BORDER | NK_WINDOW_SCROLL_AUTO_HIDE))
        {
            DrawSecondSelection();
            nk_end(ctx);
            nk_style_pop_style_item(ctx);
        }
    }
}

void GuiCombat::invokeSceneChange()
{
}

void GuiCombat::drawStartRoundWindow()
{
    if (nk_begin(ctx, "Background",
        nk_rect(0, 0,
            windowWidth, windowHeight),
        NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR))nk_end(ctx);
    if (nk_begin(ctx, "Round ",
        nk_rect(windowWidth / 4,  windowHeight / 4,
            windowWidth / 2, windowHeight/ 2),
            NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
    {
        nk_layout_row_dynamic(ctx, windowHeight/5, 2);
        auto& fonts = m_GuiHandler.GetFonts();

        nk_style_push_font(ctx, &fonts[FANTASY_MAX_SIZE]->handle);
        nk_label_colored(ctx, "Round", NK_TEXT_CENTERED, highlightColor);
        nk_label_colored(ctx, std::to_string(CombatController::roundCount).c_str(), NK_TEXT_CENTERED, highlightColor);
        nk_style_pop_font(ctx);

        nk_style_push_font(ctx, &fonts[FANTASY_VERY_LARGE]->handle);
        nk_layout_row_dynamic(ctx, windowHeight/5, 1);
        nk_label(ctx, std::to_string(static_cast<int>(countdownStartRound)).c_str(), NK_TEXT_CENTERED);
        nk_style_pop_font(ctx);
    }
    nk_end(ctx);
    if (countdownStartRound <= 0)
    {
        CombatController::setState(CombatState::INITIALIZING);
        countdownStartRound = 3.0f;
    }
    countdownStartRound -=  engine.getDeltaTime();
}

void GuiCombat::DrawDefeatWindow() {
    if (nk_begin(ctx, "Background",
        nk_rect(0, 0,
            windowWidth, windowHeight),
        NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR))nk_end(ctx);
    if (nk_begin(ctx, "Defeat",
        nk_rect(windowWidth / 5,  windowHeight / 5,
            windowWidth / 2, windowHeight/ 2),
            NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
    {
        nk_layout_row_dynamic(ctx, windowHeight/3, 1);
        auto& fonts = m_GuiHandler.GetFonts();
        nk_style_push_font(ctx, &fonts[FANTASY_VERY_LARGE]->handle);
        nk_label(ctx, "YOU DIED", NK_TEXT_CENTERED);
        nk_style_pop_font(ctx);
        nk_layout_row_dynamic(ctx, windowHeight/20, 3);
        nk_label(ctx, "", NK_TEXT_CENTERED);
        if (NK_WRAP::button_label(ctx, "Quit", m_Hovered, &engine))
        {
            glfwSetWindowShouldClose(engine.getWindow(), true);
        }
    }
    nk_end(ctx);
}

bool GuiCombat::DrawRewardWindow() {
    bool bAdvance = false;
    auto pLibCombat = LibCombatFunctions::GetInstance(engine);
    if (m_bFirstEnterRewardWindow) {
        m_Rewards.push_back(pLibCombat->GetRandomReward());
        m_Rewards.push_back(pLibCombat->GetRandomReward());
        m_Rewards.push_back(pLibCombat->GetRandomReward());
        m_bFirstEnterRewardWindow = false;
    }
    if (nk_begin(ctx, "Background",
        nk_rect(0, 0,
            windowWidth, windowHeight),
        NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR))nk_end(ctx);
    if (nk_begin(ctx, "Rewards",
        nk_rect(windowWidth / 4,  windowHeight / 4,
            windowWidth / 2, windowHeight/ 2),
            NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
    {
        auto& fonts = m_GuiHandler.GetFonts();
        nk_layout_row_dynamic(ctx, windowHeight / 8, 1);
        nk_style_push_font(ctx, &fonts[FANTASY_VERY_LARGE]->handle);
        nk_label_colored(ctx, "Choose a Reward: ", NK_TEXT_CENTERED, highlightColor);
        nk_style_pop_font(ctx);

        float ratio[] = {0.01, 0.1, 0.05, 0.45, 0.15, 0.2};

        nk_layout_row(ctx, NK_DYNAMIC , windowHeight / 13, 6, ratio);

        float frameDuration = 0.1f;
        int totalFrames = 4;
        int currentFrame = static_cast<int>(engine.elapsedTime / frameDuration) % totalFrames;
        auto unitImage = nk_subimage_id(m_Textures.at(m_Rewards[0].m_iUnit_ID), 192, 192,
            nk_rect(currentFrame * 48, 0, 48, 48));

        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_image(ctx, unitImage);
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_label(ctx, m_Rewards[0].m_FunctionKey.c_str(), NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(m_Rewards[0].m_iAmount).c_str(), NK_TEXT_CENTERED, numberColor);
        if (NK_WRAP::button_label(ctx, "Choose", m_Hovered, &engine))
        {
            pLibCombat->InvokeRewardCallback(m_Rewards[0]);
            m_bFirstEnterRewardWindow = true;
            bAdvance = true;
        }

        unitImage = nk_subimage_id(m_Textures.at(m_Rewards[1].m_iUnit_ID), 192, 192,
            nk_rect(currentFrame * 48, 0, 48, 48));
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_image(ctx, unitImage);
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_label(ctx, m_Rewards[1].m_FunctionKey.c_str(), NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(m_Rewards[1].m_iAmount).c_str(), NK_TEXT_CENTERED, numberColor);
        if (NK_WRAP::button_label(ctx, "Choose", m_Hovered, &engine, 1))
        {
            pLibCombat->InvokeRewardCallback(m_Rewards[1]);
            m_bFirstEnterRewardWindow = true;
            bAdvance = true;
        }

        unitImage = nk_subimage_id(m_Textures.at(m_Rewards[2].m_iUnit_ID), 192, 192,
            nk_rect(currentFrame * 48, 0, 48, 48));
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_image(ctx, unitImage);
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_label(ctx, m_Rewards[2].m_FunctionKey.c_str(), NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(m_Rewards[2].m_iAmount).c_str(), NK_TEXT_CENTERED, numberColor);
        if (NK_WRAP::button_label(ctx, "Choose", m_Hovered, &engine, 2))
        {
            pLibCombat->InvokeRewardCallback(m_Rewards[2]);
            m_bFirstEnterRewardWindow = true;
            bAdvance = true;
        }
    }
    nk_end(ctx);
    return bAdvance;
}

void GuiCombat::drawTopRow()
{
    if (!checkIfEntityHasComponent<Unit>(pInf_E, pArc_E, pCat_E, eInf_E, eArc_E, eCat_E))
    {
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_MODE
    DEBUG_LOG( << "GuiCombat::drawTopRow() Missing unit_C");
    DEBUG_LOG( << "hasComponent<Unit>(pInf_E): " << engine.componentManager.hasComponent<Unit>(pInf_E));
    DEBUG_LOG( << "hasComponent<Unit>(pArc_E): " << engine.componentManager.hasComponent<Unit>(pArc_E));
    DEBUG_LOG( << "hasComponent<Unit>(pCat_E): " << engine.componentManager.hasComponent<Unit>(pCat_E));
    DEBUG_LOG( << "hasComponent<Unit>(eInf_E): " << engine.componentManager.hasComponent<Unit>(eInf_E));
    DEBUG_LOG( << "hasComponent<Unit>(eArc_E): " << engine.componentManager.hasComponent<Unit>(eArc_E));
    DEBUG_LOG( << "hasComponent<Unit>(eCat_E): " << engine.componentManager.hasComponent<Unit>(eCat_E));
#endif
}
////////////////////////////////////////////////////////////////////////

        return;
    }
    if (!checkIfEntityHasComponent<SiegeEngine>(pCat_E, eCat_E))
    {
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG_MODE
    DEBUG_LOG( << "GuiCombat::drawTopRow() Missing siege_C");
    DEBUG_LOG( << "hasComponent<Unit>(pInf_E): " << engine.componentManager.hasComponent<Unit>(pInf_E));
    DEBUG_LOG( << "hasComponent<Unit>(pArc_E): " << engine.componentManager.hasComponent<Unit>(pArc_E));
    DEBUG_LOG( << "hasComponent<Unit>(pCat_E): " << engine.componentManager.hasComponent<Unit>(pCat_E));
    DEBUG_LOG( << "hasComponent<Unit>(eInf_E): " << engine.componentManager.hasComponent<Unit>(eInf_E));
    DEBUG_LOG( << "hasComponent<Unit>(eArc_E): " << engine.componentManager.hasComponent<Unit>(eArc_E));
    DEBUG_LOG( << "hasComponent<Unit>(eCat_E): " << engine.componentManager.hasComponent<Unit>(eCat_E));
#endif
}
////////////////////////////////////////////////////////////////////////
        return;
    }
    float ratio[] = {0.05, 0.01,  0.1, 0.2, 0.01, 0.04, 0.2, 0.04, 0.01,  0.2, 0.1 };
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight/20, 11, ratio);

    if (NK_WRAP::button_label(ctx, "esc", m_Hovered, &engine))
    {
        // endScene = true;
    }



    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer


    auto& pInfU_C = engine.componentManager.getComponent<Unit>(pInf_E);
    auto& pArcU_C = engine.componentManager.getComponent<Unit>(pArc_E);
    auto& pCatSE_C = engine.componentManager.getComponent<SiegeEngine>(pCat_E);

    auto currentInf = pInfU_C.totalAmount;
    auto currentArc = pArcU_C.totalAmount;
    auto currentCat = pCatSE_C.m_iUsedAmount * pCatSE_C.cost;
    auto currenTotal = currentInf + currentArc + currentCat;
    auto maxAmount = pInfU_C.lifetimeMaxAmount + pArcU_C.lifetimeMaxAmount;

    auto hp = static_cast<unsigned long long>(100.0f * currenTotal/maxAmount);
    nk_label(ctx, "Player", NK_TEXT_LEFT);
    nk_progress(ctx, &hp, 100, NK_FIXED);

    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
    switch (m_PlaybackState) {
    case PlaybackState::DEFAULT: {
        if (NK_WRAP::button_label(ctx, ">>", m_Hovered, &engine)) {
            m_PlaybackState = PlaybackState::TWICE;
            engine.SetSpeedUpValue(2.0f);
        }
        break;
    }
    case PlaybackState::TWICE: {
        if (NK_WRAP::button_label(ctx, ">>>>", m_Hovered, &engine)) {
            m_PlaybackState = PlaybackState::MAX;
            engine.SetSpeedUpValue(4.0f);
        }
        break;
    }
    case PlaybackState::MAX: {
        if (NK_WRAP::button_label(ctx, ">", m_Hovered, &engine)) {
            m_PlaybackState = PlaybackState::DEFAULT;
            engine.SetSpeedUpValue(1.0f);
        }
        break;
    }
    default: {
        break;
    }
    }

    std::string roundLabel = "Round  " + std::to_string(CombatController::roundCount);
    nk_label_colored(ctx, roundLabel.c_str(), NK_TEXT_CENTERED, highlightColor);
    nk_label(ctx, "", NK_TEXT_CENTERED);
    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer

    auto& eInfU_C = engine.componentManager.getComponent<Unit>(eInf_E);
    auto& eArcU_C = engine.componentManager.getComponent<Unit>(eArc_E);
    auto& eCatSE_C = engine.componentManager.getComponent<SiegeEngine>(eCat_E);

    currentInf = eInfU_C.totalAmount;
    currentArc = eArcU_C.totalAmount;
    currentCat = eCatSE_C.m_iUsedAmount * eCatSE_C.cost;
    currenTotal = currentInf + currentArc + currentCat;
    maxAmount = eInfU_C.lifetimeMaxAmount + eArcU_C.lifetimeMaxAmount;
    hp = static_cast<unsigned long long>(100.0f * currenTotal/maxAmount);
    nk_progress(ctx, &hp, 100, NK_FIXED);
    nk_label(ctx, "Enemy", NK_TEXT_RIGHT);
}

void GuiCombat::drawEndTurnWindow()
{
    auto pCombatSelection = CombatSelection::GetInstance();
    float ratio[] = {0.5, 0.4};
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight/20, 2, ratio);
    std::string turnLabel =  "Turn  " + std::to_string(CombatController::turnCount);
    nk_label(ctx, turnLabel.c_str(), NK_TEXT_LEFT);
    // struct nk_image skip = GuiHandler::getTileImage(textureID, 9, 5, 1, 1, 3072, 3072);
    // nk_button_image(ctx, skip);
    if (NK_WRAP::button_label(ctx, "Next", m_Hovered, &engine) && CombatController::getCombatState() == CombatState::MAIN_PHASE ) // CombatController.getState == IDLE
    {
        CombatController::setState(CombatState::ANIMATION);
        pCombatSelection->m_pFirstUnit_C = nullptr;
        pCombatSelection->m_pSecondUnit_C = nullptr;
    }
}

void GuiCombat::DrawFirstSelection() const {
    auto& fonts = m_GuiHandler.GetFonts();
    auto pCombatSelection = CombatSelection::GetInstance();
    auto pFirstSelection = pCombatSelection->m_pFirstUnit_C;
    auto tagOwnerFirstSelection = engine.componentManager.getComponent<TagComponent>(pFirstSelection->entity()).value;

    nk_layout_row_dynamic(ctx, windowHeight/20, 2);
    nk_label_colored(ctx, getType(*pFirstSelection).c_str(), NK_TEXT_LEFT, highlightColor);
    if (tagOwnerFirstSelection == Tag::PLAYER)
    {
        nk_label_colored(ctx, getOwner(tagOwnerFirstSelection).c_str(), NK_TEXT_LEFT, playerColor);
    }else
    {
        nk_label_colored(ctx, getOwner(tagOwnerFirstSelection).c_str(), NK_TEXT_LEFT, enemyColor);
    }

    nk_layout_row_dynamic(ctx, windowHeight/40, 2);
    nk_style_push_font(ctx, &fonts[FANTASY_SMALL]->handle);
    {
        nk_label(ctx, "Total  Left: ", NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(pFirstSelection->totalAmount).c_str(), NK_TEXT_LEFT, numberColor);
        if (tagOwnerFirstSelection == Tag::PLAYER)
        {
            nk_label(ctx, "Left  this  turn: ", NK_TEXT_LEFT);
            nk_label_colored(ctx, std::to_string(pFirstSelection->availableAmount).c_str(), NK_TEXT_LEFT, playerColor);
            if (engine.componentManager.hasComponent<SiegeEngine>(pFirstSelection->entity())) {
                auto& siege_C = engine.componentManager.getComponent<SiegeEngine>(pFirstSelection->entity());
                int iUsedAmount = siege_C.m_iUsedAmountNew + siege_C.m_iUsedAmount;
                int iUnusedAmount = pFirstSelection->totalAmount - iUsedAmount;
                nk_label(ctx, "Unused: ", NK_TEXT_LEFT);
                nk_label_colored(ctx, std::to_string(iUnusedAmount).c_str(), NK_TEXT_LEFT, ColorOrange);

                std::string pTextCost = std::to_string(siege_C.cost) + "  other  units  necessary  per  individual  Catapult.";
                nk_layout_row_dynamic(ctx, windowHeight/20, 1);
                nk_label_colored_wrap(ctx, pTextCost.c_str(), numberColor);
                nk_layout_row_dynamic(ctx, windowHeight/40, 2);
            }
        }

        nk_label_colored(ctx, "Stats Single Unit", NK_TEXT_LEFT, highlightColor);
        nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer

        nk_label(ctx, "Atk Delay: ", NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(static_cast<int>(pFirstSelection->speed)).c_str(), NK_TEXT_LEFT, ColorOrange);

        nk_label(ctx, "HP: ",NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(static_cast<int>(pFirstSelection->hpValue)).c_str(), NK_TEXT_LEFT, numberColor);

        nk_label(ctx, "Def: ",NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(static_cast<int>(pFirstSelection->armorValue)).c_str(), NK_TEXT_LEFT, numberColor);

        nk_label(ctx, "Atk: ", NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(static_cast<int>(pFirstSelection->attackValue)).c_str(), NK_TEXT_LEFT, numberColor);

        nk_label(ctx, "Accuracy: ", NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(pFirstSelection->accuracy).c_str(), NK_TEXT_LEFT, numberColor);

        nk_label(ctx, "Crit Chance: ", NK_TEXT_LEFT);
        nk_label_colored(ctx, std::to_string(static_cast<int>(pFirstSelection->critChance)).c_str(), NK_TEXT_LEFT, numberColor);
    }
    nk_style_pop_font(ctx);
}

void GuiCombat::DrawSecondSelection() {
    auto& fonts = m_GuiHandler.GetFonts();
    auto pCombatSelection = CombatSelection::GetInstance();
    auto pFirstSelection = pCombatSelection->m_pFirstUnit_C;
    auto pSecondSelection = pCombatSelection->m_pSecondUnit_C;

    bool bFirstSelctedIsCatapult = engine.componentManager.hasComponent<SiegeEngine>(pFirstSelection->entity());
    auto tagOwnerSecondSelection = engine.componentManager.getComponent<TagComponent>(pSecondSelection->entity()).value;

    float ratio[] = {0.1, 0.7, 0.2};
    nk_layout_row_dynamic(ctx, windowHeight/25, 2);
    nk_label_colored(ctx, getType(*pSecondSelection).c_str(), NK_TEXT_LEFT, highlightColor);

    if (tagOwnerSecondSelection == Tag::PLAYER && !bFirstSelctedIsCatapult)
    {
        const auto& siege_C = engine.componentManager.getComponent<SiegeEngine>(pSecondSelection->entity());
        const auto cost = siege_C.cost;
        const int iUnusedAmount = pSecondSelection->totalAmount - siege_C.m_iUsedAmount - siege_C.m_iUsedAmountNew;
        const int iCanUseAmount = pFirstSelection->availableAmount / cost;

        nk_label_colored(ctx, "Player", NK_TEXT_CENTERED, playerColor);
        nk_style_push_font(ctx, &fonts[FANTASY_SMALL]->handle);
        {
            nk_label(ctx, "Unused Left: ", NK_TEXT_LEFT);
            nk_label_colored(ctx, std::to_string(iUnusedAmount).c_str(), NK_TEXT_LEFT, ColorOrange);
            nk_layout_row(ctx,NK_DYNAMIC,  windowHeight/30, 3, ratio);
            nk_label(ctx, std::to_string(value).c_str(),NK_TEXT_CENTERED);
            if(pSecondSelection->totalAmount*cost <= pFirstSelection->totalAmount)
            {
                NK_WRAP::slider_int(ctx, 0, &value, iUnusedAmount, 1, "UnusedAmount", m_Hovered, &engine);
                if (NK_WRAP::button_label(ctx, "All", m_Hovered, &engine, 1)) {
                    value = iUnusedAmount;
                }
            }
            else
            {
                NK_WRAP::slider_int(ctx, 0, &value, iCanUseAmount, 1, "CanUseAmount", m_Hovered, &engine);
                if (NK_WRAP::button_label(ctx, "All", m_Hovered, &engine, 2)) {
                    value = iCanUseAmount;
                }
            }

            nk_layout_row_dynamic(ctx, windowHeight/30, 3);
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
            if (NK_WRAP::button_label(ctx, "Use", m_Hovered, &engine))
            {
                pCombatSelection->InvokeUse(pFirstSelection->category, value, pSecondSelection->category);
                pCombatSelection->ResetSelections();
                value = 0;
            }
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
        }
        nk_style_pop_font(ctx);
    }
    else
    {
        int availableTroups = pFirstSelection->availableAmount;

        nk_label_colored(ctx, "Enemy", NK_TEXT_CENTERED, enemyColor);
        nk_style_push_font(ctx, &fonts[FANTASY_SMALL]->handle);
        {
            nk_label(ctx, "Total  Left: ", NK_TEXT_LEFT);
            nk_label_colored(ctx, std::to_string(pFirstSelection->totalAmount).c_str(), NK_TEXT_LEFT, numberColor);

            nk_layout_row(ctx,NK_DYNAMIC,  windowHeight/30, 3, ratio);
            nk_label(ctx, std::to_string(value).c_str(),NK_TEXT_CENTERED);
            NK_WRAP::slider_int(ctx, 0, &value, availableTroups, 1, "AttackSlider", m_Hovered, &engine);
            if (NK_WRAP::button_label(ctx, "All", m_Hovered, &engine, 3)) {
                value = availableTroups;
            }

            nk_layout_row_dynamic(ctx, windowHeight/30, 3);
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
            if (NK_WRAP::button_label(ctx, "Attack", m_Hovered, &engine))
            {
                pCombatSelection->InvokeAttack(pFirstSelection->category, value, pSecondSelection->category);
                pCombatSelection->ResetSelections();
                value = 0;
            }
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
        }
        nk_style_pop_font(ctx);
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
                pInf_E = unit.entity();
            }else if (unit.category == UnitCategory::ARCHER)
            {
                pArc_E = unit.entity();
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                pCat_E = unit.entity();
            }
        }else if (tag == Tag::ENEMY)
        {
            if (unit.category == UnitCategory::INFANTRY)
            {
                eInf_E = unit.entity();
            }else if (unit.category == UnitCategory::ARCHER)
            {
                eArc_E = unit.entity();
            }
            else if (unit.category == UnitCategory::CATAPULT)
            {
                eCat_E = unit.entity();
            }
        }
    });
    m_Textures[pInf_E] = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png", false);
    m_Textures[pArc_E] = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png", false);
    m_Textures[pCat_E] = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png", false);
}
