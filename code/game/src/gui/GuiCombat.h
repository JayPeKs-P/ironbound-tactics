//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include "engine/ecs/Gui.h"
#include "../components/CombatSelection.h"

namespace gl3 {
    struct RandomReward;
    class GuiHandler;
}

using gl3::engine::ecs::Gui;
enum class PlaybackState{DEFAULT, TWICE, MAX};

class GuiCombat: public Gui {
    using event_t = gl3::engine::events::Event<GuiCombat>;
public:
    static event_t startRound;
    static event_t quitToMenu;
    GuiCombat(gl3::engine::Game& game, gl3::GuiHandler& guiHandler,  nk_context* ctx, nk_uint& textureID);
    ~GuiCombat();

    static float countdownStartRound;
private:
    void render() override;
    void invokeSceneChange() override;
    void drawStartRoundWindow();
    void DrawDefeatWindow();
    bool DrawRewardWindow();
    void drawTopRow();
    void drawEndTurnWindow();
    void DrawFirstSelection() const;
    void DrawSecondSelection();
    void DrawPauseWindow();
    void getComponents(gl3::engine::Game& game);
    void HelperLoadConfig();
    void HelperNewHighscore(int iValue) const;
    void HelperResetRewardSelection();

    guid_t pInf_E;
    guid_t pArc_E;
    guid_t pCat_E;

    guid_t eInf_E;
    guid_t eArc_E;
    guid_t eCat_E;

    // struct nk_rect infoBounds;
    nk_color ColorYellow = nk_rgba(255, 250, 200, 255);
    nk_color ColorGreen = nk_rgba(100, 255, 100, 255);
    nk_color ColorRed = nk_rgba(255, 100, 100, 255);
    nk_color ColorBlue = nk_rgba(100, 200, 255, 255);
    nk_color ColorOrange = nk_rgba(255, 165, 0, 255);

    int value = 0;
    bool m_bPaused = false;
    bool m_bNewHighscore = false;
    uint8_t m_CurrentHighscore = -1;
    std::string m_HighscoreText;

    bool justStarted = true;
    gl3::GuiHandler& m_GuiHandler;
    std::vector<gl3::RandomReward> m_Rewards;
    std::unordered_map<guid_t, unsigned int> m_Textures;

    PlaybackState m_PlaybackState = PlaybackState::DEFAULT;
    bool m_bFirstRewardSelected = false;
    bool m_bSecondRewardSelected = false;
    bool m_bThirdRewardSelected = false;
    bool m_bFourthRewardSelected = false;

    bool m_bNothingSelected = true;
    bool m_bFirstEnterRewardWindow = true;
};
