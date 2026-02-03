//
// Created by Julian Kalb on 02/02/2026.
//

#include "GuiMainMenu.h"

#include <fstream>

#include "json.hpp"
#include "../systems/CombatController.h"
#include "../systems/GuiHandler.h"
#include "engine/Texture.h"
#include "../components/TagComponent.h"
#include "engine/rendering/Model2D.h"


GuiMainMenu::event_t GuiMainMenu::onPressPlay;
using namespace gl3;
GuiMainMenu::GuiMainMenu(engine::Game& game, GuiHandler& guiHandler, nk_context* ctx, nk_uint& textureID):
Gui(game, ctx, textureID),
m_GuiHandler(guiHandler)
{
    auto pSoundSystem = engine::SoundSystem::GetInstance();
    pSoundSystem->PlayMusic(engine::MUSIC_MAIN_MENU);
    HelperLoadConfig();
}

GuiMainMenu::~GuiMainMenu() {
    DeleteSkinSelection();
}

void GuiMainMenu::render() {
    if (nk_begin(ctx, "Background",
        nk_rect(0, 0,
            windowWidth, windowHeight),
        NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR))nk_end(ctx);
    switch (m_ActiveState) {
    case MainMenuState::MAIN_MENU: {
        if (nk_begin(ctx, "Main Menu Buttons",
            nk_rect(windowWidth * 2/6,  windowHeight * 3/ 6,
                windowWidth *  2/6 , windowHeight * 1/2 ),
                NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
        {
            MainDisplay();
            nk_end(ctx);
        }
        break;
    }
    case MainMenuState::SETTINGS: {
        if (nk_begin(ctx, "Settings Menu",
            nk_rect(windowWidth * 2/6,  windowHeight * 3/ 6,
                windowWidth *  2/6 , windowHeight * 11/24 ),
                NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
        {
            SettingsDisplay();
            nk_end(ctx);
        }
        break;
    }
    case MainMenuState::SKIN_SELECTION: {
        if (nk_begin(ctx, "Unlocked  Skins",
            nk_rect(windowWidth / 4,  windowHeight / 4,
                windowWidth / 2, windowHeight * 3/5),
                 NK_WINDOW_BORDER | NK_WINDOW_BORDER))
        {
            SkinSelecitonDisplay();
            nk_end(ctx);
        }
        break;
    }
    case MainMenuState::TUTORIAL: {
        break;
    }
    }
}

void GuiMainMenu::invokeSceneChange() {
    onPressPlay.invoke();
}

void GuiMainMenu::MainDisplay() {
    auto& fonts = m_GuiHandler.GetFonts();
    nk_layout_row_dynamic(ctx, windowHeight / 50, 1);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, windowHeight / 18, 1);
    nk_style_push_font(ctx, &fonts[FANTASY_LARGE]->handle);
    nk_label_colored(ctx, "Current Highscore: ", NK_TEXT_CENTERED, ColorYellow);
    nk_style_pop_font(ctx);
    nk_layout_row_dynamic(ctx, windowHeight / 36, 1);
    nk_label_colored(ctx, m_HighscoreText.c_str(), NK_TEXT_CENTERED, ColorBlue);
    nk_layout_row_dynamic(ctx, windowHeight / 50, 1);
    nk_label(ctx, "", NK_TEXT_LEFT);

    float ratio[] = {0.2, 0.6, 0.2};
    nk_layout_row(ctx, NK_DYNAMIC, windowHeight / 18, 3, ratio);
    nk_label(ctx, "", NK_TEXT_LEFT);
    if (NK_WRAP::button_label(ctx, "Play", m_Hovered, &engine)) {
        endScene = true;
    }
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_label(ctx, "", NK_TEXT_LEFT);
    if (NK_WRAP::button_label(ctx, "Settings", m_Hovered, &engine)) {
        SetMenuState(MainMenuState::SETTINGS);
    }
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_label(ctx, "", NK_TEXT_LEFT);
    if (NK_WRAP::button_label(ctx, "Change Skin", m_Hovered, &engine)) {
        SetMenuState(MainMenuState::SKIN_SELECTION);
        UpdateUnitGUID();
        GetCurrentTextureID();
        LoadSkinSelection();
    }
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_label(ctx, "", NK_TEXT_LEFT);
    if (NK_WRAP::button_label(ctx, "How to Play", m_Hovered, &engine)) {
        SetMenuState(MainMenuState::TUTORIAL);
    }
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_label(ctx, "", NK_TEXT_LEFT);
    if (NK_WRAP::button_label(ctx, "Quit", m_Hovered, &engine)) {
        glfwSetWindowShouldClose(engine.getWindow(), true);
    }
    nk_label(ctx, "", NK_TEXT_LEFT);
}

void GuiMainMenu::SettingsDisplay() {
    auto& fonts = m_GuiHandler.GetFonts();
    nk_layout_row_dynamic(ctx, windowHeight / 18, 1);
    nk_style_push_font(ctx, &fonts[FANTASY_LARGE]->handle);
    nk_label_colored(ctx, "Settings", NK_TEXT_CENTERED, ColorYellow);
    nk_style_pop_font(ctx);

    nk_layout_row_dynamic(ctx, windowHeight / 50, 1);
    nk_label(ctx, "", NK_TEXT_LEFT);

    float ratio[] = {0.05f, 0.325f, 0.53f, 0.05f};
    nk_layout_row(ctx, NK_DYNAMIC, windowHeight / 18, 4, ratio);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "Volume", NK_TEXT_LEFT);
    auto pSoundSystem = engine::SoundSystem::GetInstance();
    float fCurrentVolume = pSoundSystem->GetVolume();
    float newValue = NK_WRAP::slider_float(ctx, 0.0f, fCurrentVolume, 1.0f, 0.05f, "Volume", m_Hovered, &engine);
    pSoundSystem->SetVolume(newValue);
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, windowHeight / 50, 1);
    nk_label(ctx, "", NK_TEXT_LEFT);

    float toggleRatio[] = {0.05f, 0.325f, 0.13f, 0.27f, 0.13f, 0.05f};
    nk_layout_row(ctx, NK_DYNAMIC, windowHeight / 18, 6, toggleRatio);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "Fullscreen", NK_TEXT_LEFT);
    nk_label(ctx, "", NK_TEXT_LEFT);
    if (NK_WRAP::button_label(ctx, "Toggle", m_Hovered, &engine)) {
        engine.ToggleFullScreen();
    }
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, windowHeight / 10, 1);
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, windowHeight / 18, 5);
    nk_label(ctx, "", NK_TEXT_LEFT);
    nk_label(ctx, "", NK_TEXT_LEFT);
    if (NK_WRAP::button_label(ctx, "Back", m_Hovered, &engine)) {
        SetMenuState(MainMenuState::MAIN_MENU);
    }
}

void GuiMainMenu::SkinSelecitonDisplay() {
    auto& fonts = m_GuiHandler.GetFonts();
    nk_layout_row_dynamic(ctx, windowHeight / 24, 1);
    nk_style_push_font(ctx, &fonts[FANTASY_LARGE]->handle);
    {
        nk_label_colored(ctx, "Skin  Collection", NK_TEXT_CENTERED, ColorYellow);
    }
    nk_style_pop_font(ctx);

    nk_layout_row_dynamic(ctx, windowHeight / 18, 3);
    if (NK_WRAP::button_label(ctx, "Infantry", m_Hovered, &engine)) {
        SetSkinSelection(UnitSelection::INFANTRY);
        LoadSkinSelection();
    }
    if (NK_WRAP::button_label(ctx, "Archer", m_Hovered, &engine)) {
        SetSkinSelection(UnitSelection::ARCHER);
        LoadSkinSelection();
    }
    if (NK_WRAP::button_label(ctx, "Catapult", m_Hovered, &engine)) {
        SetSkinSelection(UnitSelection::CATAPULT);
        LoadSkinSelection();
    }
    nk_layout_row_dynamic(ctx, windowHeight / 24, 1);
    nk_label(ctx, "", NK_TEXT_LEFT);

    nk_style_push_font(ctx, &fonts[FANTASY_SMALL]->handle);
    {
        float ratio[] = {0.01, 0.1, 0.01,  0.7, 0.18f};
        nk_layout_row(ctx, NK_DYNAMIC , windowHeight / 13, 5, ratio);

        float frameDuration = 0.1f;
        int totalFrames = 4;
        int currentFrame = static_cast<int>(engine.elapsedTime / frameDuration) % totalFrames;

        auto unitImage = nk_subimage_id(iSkinBasic, 192, 192,
            nk_rect(currentFrame * 48, 0, 48, 48));
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_image(ctx, unitImage);
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_label_colored(ctx, "Active", NK_TEXT_CENTERED, ColorYellow);
        if (NK_WRAP::button_label(ctx, "Select", m_Hovered, &engine)) {
            switch (m_SkinSelection) {
            case UnitSelection::INFANTRY: {
                UpdateInfantryTexture(SkinType::BASIC);
                break;
            }
            case UnitSelection::ARCHER: {
                UpdateArcherTexture(SkinType::BASIC);
                break;
            }
            case UnitSelection::CATAPULT: {
                UpdateCatapultTexture(SkinType::BASIC);
                break;
            }
            default: {}
            }
        }

        unitImage = nk_subimage_id(iSkinEasy, 192, 192,
                    nk_rect(currentFrame * 48, 0, 48, 48));
        std::string LabelText = "Reach  Round  " +
            std::to_string(BOUND_EASY) +
            "  for  the  first  time  to  unlock  this  skin.";
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_image(ctx, unitImage);
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_label_wrap(ctx, LabelText.c_str());
        if (m_CurrentHighscore >= BOUND_EASY) {
            if (NK_WRAP::button_label(ctx, "Select", m_Hovered, &engine, 1)) {
                switch (m_SkinSelection) {
                case UnitSelection::INFANTRY: {
                    UpdateInfantryTexture(SkinType::EASY);
                    break;
                }
                case UnitSelection::ARCHER: {
                    UpdateArcherTexture(SkinType::EASY);
                    break;
                }
                case UnitSelection::CATAPULT: {
                    UpdateCatapultTexture(SkinType::EASY);
                    break;
                }
                default: {}
                }
            }
        }
        else {
            nk_label_colored(ctx, "Locked", NK_TEXT_CENTERED, ColorRed);
        }

        unitImage = nk_subimage_id(iSkinMedium, 192, 192,
                    nk_rect(currentFrame * 48, 0, 48, 48));
        LabelText = "Reach  Round  " +
            std::to_string(BOUND_MEDIUM) +
            "  for  the  first  time  to  unlock  this  skin.";
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_image(ctx, unitImage);
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_label_wrap(ctx, LabelText.c_str());
        if (m_CurrentHighscore >= BOUND_MEDIUM) {
            if (NK_WRAP::button_label(ctx, "Select", m_Hovered, &engine, 2)) {
                switch (m_SkinSelection) {
                case UnitSelection::INFANTRY: {
                    UpdateInfantryTexture(SkinType::MEDIUM);
                    break;
                }
                case UnitSelection::ARCHER: {
                    UpdateArcherTexture(SkinType::MEDIUM);
                    break;
                }
                case UnitSelection::CATAPULT: {
                    UpdateCatapultTexture(SkinType::MEDIUM);
                    break;
                }
                default: {}
                }
            }
        }
        else {
            nk_label_colored(ctx, "Locked", NK_TEXT_CENTERED, ColorRed);
        }

        unitImage = nk_subimage_id(iSkinHard, 192, 192,
                    nk_rect(currentFrame * 48, 0, 48, 48));
        LabelText = "Reach  Round  " +
            std::to_string(BOUND_HARD) +
            "  for  the  first  time  to  unlock  this  skin.";
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_image(ctx, unitImage);
        nk_label(ctx, "", NK_TEXT_LEFT);
        nk_label_wrap(ctx, LabelText.c_str());
        if (m_CurrentHighscore >= BOUND_HARD) {
            if (NK_WRAP::button_label(ctx, "Select", m_Hovered, &engine, 3)) {
                switch (m_SkinSelection) {
                case UnitSelection::INFANTRY: {
                    UpdateInfantryTexture(SkinType::HARD);
                    break;
                }
                case UnitSelection::ARCHER: {
                    UpdateArcherTexture(SkinType::HARD);
                    break;
                }
                case UnitSelection::CATAPULT: {
                    UpdateCatapultTexture(SkinType::HARD);
                    break;
                }
                default: {}
                }
            }
        }
        else {
            nk_label_colored(ctx, "Locked", NK_TEXT_CENTERED, ColorRed);
        }

        nk_layout_row_dynamic(ctx, windowHeight / 18, 3);
        nk_label(ctx, "", NK_TEXT_LEFT);
        if (NK_WRAP::button_label(ctx, "Return  To  Menu", m_Hovered, &engine)) {
            SetMenuState(MainMenuState::MAIN_MENU);
            SetSkinSelection(UnitSelection::INFANTRY);
            DeleteSkinSelection();
        }
    }
    nk_style_pop_font(ctx);
}

void GuiMainMenu::SetSkinSelection(UnitSelection newSkinSelection) {
    m_SkinSelection = newSkinSelection;
}

void GuiMainMenu::LoadSkinSelection() {
    DeleteSkinSelection();

    switch (m_SkinSelection) {
    case UnitSelection::INFANTRY: {
        iSkinBasic = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png", false);
        iSkinEasy = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_01_Idle.png", false);
        iSkinMedium = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_04_Idle.png", false);
        iSkinHard = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_02_Idle.png", false);
        break;
    }
    case UnitSelection::ARCHER: {
        iSkinBasic = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png", false);
        iSkinEasy = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_01_Idle.png", false);
        iSkinMedium = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_04_Idle.png", false);
        iSkinHard = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_02_Idle.png", false);
        break;
    }
    case UnitSelection::CATAPULT: {
        iSkinBasic = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png", false);
        iSkinEasy = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_01_Idle.png", false);
        iSkinMedium = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_04_Idle.png", false);
        iSkinHard = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_02_Idle.png", false);
        break;
    }
    default: {}
    }
}

void GuiMainMenu::DeleteSkinSelection() {
    if (iSkinBasic != GL_MAX_INTEGER_SAMPLES) {
        glDeleteTextures(1, &iSkinBasic);
        iSkinBasic = GL_MAX_INTEGER_SAMPLES;
    }
    if (iSkinEasy != GL_MAX_INTEGER_SAMPLES) {
        glDeleteTextures(1, &iSkinEasy);
        iSkinEasy = GL_MAX_INTEGER_SAMPLES;
    }
    if (iSkinMedium != GL_MAX_INTEGER_SAMPLES) {
        glDeleteTextures(1, &iSkinMedium);
        iSkinMedium = GL_MAX_INTEGER_SAMPLES;
    }
    if (iSkinHard != GL_MAX_INTEGER_SAMPLES) {
        glDeleteTextures(1, &iSkinHard);
        iSkinHard = GL_MAX_INTEGER_SAMPLES;
    }
}

void GuiMainMenu::UpdateUnitGUID() {
    engine.componentManager.forEachComponent<Unit>([&](Unit &unit)
    {
        auto &tag = engine.componentManager.getComponent<TagComponent>(unit.entity()).value;
        if (tag != Tag::PLAYER) return;
        switch (unit.category)
        {
        case UnitCategory::INFANTRY:
            m_iInfantryPlayerID = unit.entity();
            break;
        case UnitCategory::ARCHER:
            m_iArcherPlayerID = unit.entity();
            break;
        case UnitCategory::CATAPULT:
            m_iCatapultPlayerID = unit.entity();
            break;
        }
    });
}

void GuiMainMenu::GetCurrentTextureID() {
    m_iInfantryTextureID = engine.componentManager.getComponent<Model2D>(m_iInfantryPlayerID).texture;
    m_iArcherTextureID = engine.componentManager.getComponent<Model2D>(m_iArcherPlayerID).texture;
    m_iCatapultTextureID = engine.componentManager.getComponent<Model2D>(m_iCatapultPlayerID).texture;
}

void GuiMainMenu::UpdateInfantryTexture(SkinType newSkin) {
    auto pUnit = &engine.componentManager.getComponent<Model2D>(m_iInfantryPlayerID);
    glDeleteTextures(1, &pUnit->texture);
    switch (newSkin) {
    case SkinType::BASIC: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png");
        break;
    }
    case SkinType::EASY: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_01_Idle.png");
        break;
    }
    case SkinType::MEDIUM: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_04_Idle.png");
        break;
    }
    case SkinType::HARD: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_02_Idle.png");
        break;
    }
    default: {}
    }
}

void GuiMainMenu::UpdateArcherTexture(SkinType newSkin) {
    auto pUnit = &engine.componentManager.getComponent<Model2D>(m_iArcherPlayerID);
    glDeleteTextures(1, &pUnit->texture);
    switch (newSkin) {
    case SkinType::BASIC: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png");
        break;
    }
    case SkinType::EASY: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_01_Idle.png");
        break;
    }
    case SkinType::MEDIUM: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_04_Idle.png");
        break;
    }
    case SkinType::HARD: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_02_Idle.png");
        break;
    }
    default: {}
    }
}

void GuiMainMenu::UpdateCatapultTexture(SkinType newSkin) {
    auto pUnit = &engine.componentManager.getComponent<Model2D>(m_iCatapultPlayerID);
    glDeleteTextures(1, &pUnit->texture);
    switch (newSkin) {
    case SkinType::BASIC: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png");
        break;
    }
    case SkinType::EASY: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_01_Idle.png");
        break;
    }
    case SkinType::MEDIUM: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_04_Idle.png");
        break;
    }
    case SkinType::HARD: {
        pUnit->texture = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_02_Idle.png");
        break;
    }
    default: {}
    }
}

void GuiMainMenu::HelperLoadConfig() {
    auto pConfigPath = engine.GetConfigPath();
    using json = nlohmann::json;
    json config;

    std::ifstream in(pConfigPath);
    in >> config;
    m_CurrentHighscore = config["highscore"];
    m_HighscoreText = "Round " + std::to_string(m_CurrentHighscore);
}
