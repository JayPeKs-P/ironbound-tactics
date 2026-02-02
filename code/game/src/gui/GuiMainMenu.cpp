//
// Created by Julian Kalb on 02/02/2026.
//

#include "GuiMainMenu.h"

#include <fstream>

#include "json.hpp"
#include "../systems/GuiHandler.h"

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

void GuiMainMenu::render() {
    if (nk_begin(ctx, "Background",
        nk_rect(0, 0,
            windowWidth, windowHeight),
        NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR))nk_end(ctx);
    switch (m_ActiveState) {
    case MainMenuState::MAIN_MENU: {
        if (nk_begin(ctx, "Main Menu Buttons",
            nk_rect(windowWidth * 2/6,  windowHeight * 3/ 6,
                windowWidth *  2/6 , windowHeight * 11/24 ),
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

void GuiMainMenu::HelperLoadConfig() {
    auto pConfigPath = engine.GetConfigPath();
    using json = nlohmann::json;
    json config;

    std::ifstream in(pConfigPath);
    in >> config;
    m_CurrentHighscore = config["highscore"];
    m_HighscoreText = "Round " + std::to_string(m_CurrentHighscore);
}
