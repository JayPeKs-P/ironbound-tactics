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
enum class UnitSelection {INFANTRY, ARCHER, CATAPULT};
enum class SkinType {BASIC, EASY, MEDIUM, HARD};

using gl3::engine::ecs::Gui;

class GuiMainMenu: public Gui {
    using event_t = gl3::engine::events::Event<GuiMainMenu>;
public:
    static event_t onPressPlay;
    GuiMainMenu(gl3::engine::Game& game, gl3::GuiHandler& guiHandler, nk_context* ctx, nk_uint& textureID);
    ~GuiMainMenu() override;
    void SetMenuState(MainMenuState newState) {
        m_ActiveState = newState;
    }

private:
    void render() override;
    void invokeSceneChange() override;
    void MainDisplay();
    void SettingsDisplay();
    void TutorialDisplay();
    void SkinSelecitonDisplay();
    void SetSkinSelection(UnitSelection newSkinSelection);
    void LoadSkinSelection();
    void DeleteSkinSelection();
    void UpdateUnitGUID();
    void GetCurrentTextureID();
    void UpdateInfantryTexture(SkinType newSkin);
    void UpdateArcherTexture(SkinType newSkin);
    void UpdateCatapultTexture(SkinType newSkin);
    void UpdateActiveSkinType(const std::string& sKey);
    void SetTitleVisibility(bool bValue) const; //TODO: Delete and swap for safe method

    void HelperLoadConfig();
    gl3::GuiHandler& m_GuiHandler;
    uint8_t m_CurrentHighscore;
    std::string m_HighscoreText;

    nk_color ColorYellow = nk_rgba(255, 250, 200, 255);
    nk_color ColorGreen = nk_rgba(100, 255, 100, 255);
    nk_color ColorRed = nk_rgba(255, 100, 100, 255);
    nk_color ColorBlue = nk_rgba(100, 200, 255, 255);

    MainMenuState m_ActiveState = MainMenuState::MAIN_MENU;
    UnitSelection m_SkinSelection = UnitSelection::INFANTRY;
    SkinType m_ActiveSkinType = SkinType::BASIC;

    guid_t m_iInfantryPlayerID;
    guid_t m_iArcherPlayerID;
    guid_t m_iCatapultPlayerID;

    GLuint m_iInfantryTextureID = GL_MAX_INTEGER_SAMPLES;
    GLuint m_iArcherTextureID = GL_MAX_INTEGER_SAMPLES;
    GLuint m_iCatapultTextureID = GL_MAX_INTEGER_SAMPLES;

    GLuint iSkinBasic = GL_MAX_INTEGER_SAMPLES;
    GLuint iSkinEasy = GL_MAX_INTEGER_SAMPLES;
    GLuint iSkinMedium = GL_MAX_INTEGER_SAMPLES;
    GLuint iSkinHard = GL_MAX_INTEGER_SAMPLES;

    bool m_bEasyUnlocked = false;
    bool m_bMediumUnlocked = false;
    bool m_bHardUnlocked = false;
};