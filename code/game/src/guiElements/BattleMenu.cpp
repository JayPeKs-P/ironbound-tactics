//
// Created by julia on 27/11/2024.
//

#include <iostream>

#include "battleMenu.h"


using namespace gl3;

BattleMenu::BattleMenu(const ImVec2& size):
guiElement(PngTexture("textures/gui/ui_atlas_48x48.png"), 48, 48),
size(size)
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.335f, 0.16f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.5f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.863f, 0.635f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.335f, 0.16f, 0.15f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.0);
}

void BattleMenu::renderBattleMenu()
{
    auto MAIN_WINDOW = ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoCollapse |
         ImGuiWindowFlags_NoResize;
    // // Begin ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a simple window
    ImGui::SetNextWindowPos(ImVec2(0, size.y - size.y / 3));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Background",nullptr, MAIN_WINDOW);
    drawBackground(tilesToRender, size.x, size.y / 3);
    ImGui::End();
    auto BATTLE_MENU = ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize;
    //size.x -= size.x / 2;
    ImGui::SetNextWindowPos(ImVec2(48, size.y - 192));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Battle Menu", nullptr, BATTLE_MENU);


    // Battle menu options
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 48.0f));
    if (ImGui::Button("Fight", ImVec2(144,48))) {
        lifePoints -= 0.1f;
    }

    if (ImGui::Button("Defend", ImVec2(144,48))) {

    }
    ImGui::PopStyleVar();

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(size.x /2, size.y - 192));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Life Bars", nullptr, BATTLE_MENU);
    ImGui::Text("Enemy Health:");
    ImGui::ProgressBar(lifePoints, ImVec2(244.0f, 24.0f));
    ImGui::End();

    // Render ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
