//
// Created by Julian Kalb on 02/02/2026.
//

#pragma once
#include "engine/ecs/Gui.h"
#include "engine/Events.h"

namespace gl3 {
    class GuiHandler;
}
enum class MainMenuState {MAIN_MENU, SETTINGS, SKIN_SELECTION, TUTORIAL};

using gl3::engine::ecs::Gui;

class GuiMainMenu: public Gui {
    using event_t = gl3::engine::events::Event<GuiMainMenu>;
public:
    static event_t onPressPlay;
    GuiMainMenu(gl3::engine::Game& game, gl3::GuiHandler& guiHandler, nk_context* ctx, nk_uint& textureID);
    void SetMenuState(MainMenuState newState) {
        m_ActiveState = newState;
    }

private:
    void render();
    void invokeSceneChange();
    void MainDisplay();
    void SettingsDisplay();
    void TutorialDisplay();
    void SkinSelecitonDisplay();

    void HelperLoadConfig();
    gl3::GuiHandler& m_GuiHandler;
    uint8_t m_CurrentHighscore;
    std::string m_HighscoreText;

    nk_color ColorYellow = nk_rgba(255, 250, 200, 255);
    nk_color ColorGreen = nk_rgba(100, 255, 100, 255);
    nk_color ColorRed = nk_rgba(255, 100, 100, 255);
    nk_color ColorBlue = nk_rgba(100, 200, 255, 255);
    MainMenuState m_ActiveState = MainMenuState::MAIN_MENU;
};