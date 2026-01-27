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
//     startEndOfTurn.addListener([&]()
//     {
// ////////////////////////////////////////////////////////////////////////
// {
// #ifdef DEBUG_MODE
//     DEBUG_LOG(
//         << "TRIGGERED EVENT: 'startEndOfTurn'"
//         );
// #endif
// }
// ////////////////////////////////////////////////////////////////////////
//     });

    // CombatController::enemyDead.addListener([&]()
    // {
    //     countdownStartRound = 3.0f;
    //     startRound.invoke();
    //     CombatController::setState(CombatState::WAITNEXTROUND);
    // });
    engine.PlayMusic("Retro Action Game Theme #8 (looped).wav");
}

GuiCombat::~GuiCombat() {
    for (auto val : m_Textures | std::views::values) {
        glDeleteTextures(1, &val);
    }
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
        if (DrawRewardWindow()) CombatController::setState(CombatState::PREPARE);
    }


    if (nk_begin(ctx, "Top Row Window",
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
            NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE))
    {
        nk_layout_row_dynamic(ctx, windowHeight/5, 3);
        auto& fonts = m_GuiHandler.GetFonts();
        nk_style_push_font(ctx, &fonts[FANTASY_VERY_LARGE]->handle);
        nk_label(ctx, "Round", NK_TEXT_CENTERED);
        nk_label(ctx, std::to_string(CombatController::roundCount).c_str(), NK_TEXT_CENTERED);
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
        if (NK_WRAP::button_label(ctx, "Choose", m_Hovered, &engine))
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
        if (NK_WRAP::button_label(ctx, "Choose", m_Hovered, &engine))
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
    float ratio[] = {0.05, 0.01,  0.1, 0.2, 0.3, 0.2, 0.1 };
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight/20, 7, ratio);

    if (NK_WRAP::button_label(ctx, "esc", m_Hovered, &engine))
    {
        // endScene = true;
        CombatController::setState(CombatState::VICTORY); //TODO: remove this again
    }



    nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer


    auto& pInfU_C = engine.componentManager.getComponent<Unit>(pInf_E);
    auto& pArcU_C = engine.componentManager.getComponent<Unit>(pArc_E);
    auto& pCatSE_C = engine.componentManager.getComponent<SiegeEngine>(pCat_E);

    auto currentInf = pInfU_C.totalAmount;
    auto currentArc = pArcU_C.totalAmount;
    auto currentCat = pCatSE_C.useableAmount * pCatSE_C.cost;
    auto currenTotal = currentInf + currentArc + currentCat;
    auto maxAmount = pInfU_C.lifetimeMaxAmount + pArcU_C.lifetimeMaxAmount;

    auto hp = static_cast<unsigned long long>(100.0f * currenTotal/maxAmount);
    nk_label(ctx, "Player", NK_TEXT_LEFT);
    nk_progress(ctx, &hp, 100, NK_FIXED);

    std::string roundLabel = "Round  " + std::to_string(CombatController::roundCount);
    nk_label_colored(ctx, roundLabel.c_str(), NK_TEXT_CENTERED, highlightColor);


    auto& eInfU_C = engine.componentManager.getComponent<Unit>(eInf_E);
    auto& eArcU_C = engine.componentManager.getComponent<Unit>(eArc_E);
    auto& eCatSE_C = engine.componentManager.getComponent<SiegeEngine>(eCat_E);

    currentInf = eInfU_C.totalAmount;
    currentArc = eArcU_C.totalAmount;
    currentCat = eCatSE_C.useableAmount * eCatSE_C.cost;
    currenTotal = currentInf + currentArc + currentCat;
    maxAmount = eInfU_C.lifetimeMaxAmount + eArcU_C.lifetimeMaxAmount;
    hp = static_cast<unsigned long long>(100.0f * currenTotal/maxAmount);
    nk_progress(ctx, &hp, 100, NK_FIXED);
    nk_label(ctx, "Enemy", NK_TEXT_RIGHT);
}

void GuiCombat::drawEndTurnWindow()
{
    float ratio[] = {0.5, 0.4};
    nk_layout_row(ctx, NK_DYNAMIC , windowHeight/20, 2, ratio);
    std::string turnLabel =  "Turn  " + std::to_string(CombatController::turnCount);
    nk_label(ctx, turnLabel.c_str(), NK_TEXT_LEFT);
    // struct nk_image skip = GuiHandler::getTileImage(textureID, 9, 5, 1, 1, 3072, 3072);
    // nk_button_image(ctx, skip);
    if (NK_WRAP::button_label(ctx, "Next", m_Hovered, &engine) && CombatController::getCombatState() == CombatState::MAIN_PHASE ) // CombatController.getState == IDLE
    {
        CombatController::setState(CombatState::ANIMATION);
    }
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

    bool bFirstSelctedIsCatapult = engine.componentManager.hasComponent<SiegeEngine>(unitOne->entity());

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
            if (NK_WRAP::button_label(ctx, "Attack", m_Hovered, &engine))
            {
                combatSelection_C->attack.invoke(unitOne->category, value, unitTwo->category);
                combatSelection_C->selectionOne = nullptr;
                combatSelection_C->selectionTwo = nullptr;
                value = 0;
            }
            int availableTroups = unitOne->availableAmount;
            NK_WRAP::slider_int(ctx, 0, &value, availableTroups, 1, "AttackSlider", m_Hovered, &engine);
            nk_label_colored(ctx, getType(*unitTwo).c_str(), NK_TEXT_CENTERED, enemyColor);
            nk_layout_row_dynamic(ctx, windowHeight/30, 3);

            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
            nk_label(ctx, std::to_string(value).c_str(),NK_TEXT_CENTERED);
            nk_label(ctx, "", NK_TEXT_CENTERED);    //spacer
        }
        else if (tagSelectionTwo == Tag::PLAYER && unitOne->availableAmount != 0 && !bFirstSelctedIsCatapult)
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
            if (NK_WRAP::button_label(ctx, "Use", m_Hovered, &engine))
            {
                combatSelection_C->use.invoke(unitOne->category, value, unitTwo->category);
                combatSelection_C->selectionOne = nullptr;
                combatSelection_C->selectionTwo = nullptr;
                value = 0;
            }

            if(unitTwo->totalAmount*cost <= unitOne->totalAmount)
            {
                auto unusedAmount = unitTwo->totalAmount - siege_C.useableAmount;
                NK_WRAP::slider_int(ctx, 0, &value, unusedAmount, 1, "UnusedAmount", m_Hovered, &engine);
            }
            else
            {
                int canUseAmount = unitOne->availableAmount / cost;
                NK_WRAP::slider_int(ctx, 0, &value, canUseAmount, 1, "CanUseAmount", m_Hovered, &engine);
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
    game.componentManager.forEachComponent<CombatSelection<GuiCombat>>([&] (CombatSelection<GuiCombat> &sel)
    {
        combatSelection_C = &sel;
    });
    m_Textures[pInf_E] = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png", false);
    m_Textures[pArc_E] = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png", false);
    m_Textures[pCat_E] = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png", false);
}
