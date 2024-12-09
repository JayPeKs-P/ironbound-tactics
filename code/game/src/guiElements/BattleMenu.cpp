//
// Created by julia on 27/11/2024.
//

#include <iostream>

#include "battleMenu.h"


using namespace gl3;

BattleMenu::BattleMenu(const ImVec2& size):
GuiElement(PngTexture("textures/gui/ui_atlas_48x48.png"), 48, 48),
size(size),
playerArmy(40, 11, 3, glm::vec3(0.2f,0.2f,0.0f)),
enemyArmy(36, 12, 4, glm::vec3(-0.2f, -0.2f, 0.0f))
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
    ImGui::SetNextWindowSize(ImVec2(160, 192));
    ImGui::Begin("Battle Menu", nullptr, BATTLE_MENU);


    // Battle menu options
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 16.0f));
    if (ImGui::Button("Ready", ImVec2(144,48))) {
        if (!playersTurn)
        {
            playerArmy.takeDamage(enemyArmy.dealDamage());
            playerLifePoints = playerArmy.getArmySize() / playerArmy.maxArmySize;
            enemyArmy.takeDamage(playerArmy.dealDamage());
            enemyLifePoints = enemyArmy.getArmySize() / enemyArmy.maxArmySize;
        }
        playersTurn = !playersTurn;
    }
    if (playersTurn)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.00f, 0.40f, 0.00f, 1.00f));
        ImGui::Text("\tPlayers Turn");
        ImGui::PopStyleColor();
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.00f, 0.00f, 1.00f));
        ImGui::Text("\tEnemies Turn");
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("Defend", ImVec2(144,48))) {
        if (playersTurn)
        {
            playerArmy.setDefending(Unit::Type::Infantry, infantrySelected);
            infantrySelected = 0;
            playerArmy.setDefending(Unit::Type::Archer, archersSelected);
            archersSelected = 0;
            playerArmy.setDefending(Unit::Type::Siege, siegeSelected);
            siegeSelected = 0;
        }
        else
        {
            enemyArmy.setDefending(Unit::Type::Infantry, infantrySelected);
            infantrySelected = 0;
            enemyArmy.setDefending(Unit::Type::Archer, archersSelected);
            archersSelected = 0;
            enemyArmy.setDefending(Unit::Type::Siege, siegeSelected);
            siegeSelected = 0;
        }
    }
    ImGui::PopStyleVar();

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(size.x /6, size.y - 192));
    ImGui::SetNextWindowSize(ImVec2(336, 192));
    ImGui::Begin("UnitButtons", nullptr, BATTLE_MENU);

    ImGui::Text("Infantry \nfor defense");
    ImGui::SameLine();
    if (ImGui::Button("+##infantry", ImVec2(48,48)))
    {
        infantrySelected++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-##infantry", ImVec2(48,48)) && infantrySelected > 0)
    {
        infantrySelected--;
    }
    ImGui::SameLine();
    ImGui::Text("Amount: \n%d", infantrySelected);
    ImGui::Text("Archers \nfor defense");
    ImGui::SameLine();
    if (ImGui::Button("+##archer", ImVec2(48,48)))
    {
        archersSelected++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-##archer", ImVec2(48,48)) && archersSelected > 0)
    {
        archersSelected--;
    }
    ImGui::SameLine();
    ImGui::Text("Amount: \n%d",archersSelected);
    ImGui::Text("Siege\nfor defense");
    ImGui::SameLine();
    if (ImGui::Button("+##siege", ImVec2(48,48)))
    {
        siegeSelected++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-##siege", ImVec2(48,48)) && siegeSelected > 0)
    {
        siegeSelected--;
    }
    ImGui::SameLine();
    ImGui::Text("Amount: \n%d",siegeSelected);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(size.x /2, size.y - 192));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Life Bars", nullptr, BATTLE_MENU);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.00f, 0.00f, 1.00f));
    ImGui::Text("Enemy Health:");
    ImGui::PopStyleColor();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 48.0f));
    ImGui::ProgressBar(enemyLifePoints, ImVec2(244.0f, 24.0f));
    ImGui::PopStyleVar();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.00f, 0.40f, 0.00f, 1.00f));
    ImGui::Text("Player Health:");
    ImGui::PopStyleColor();
    ImGui::ProgressBar(playerLifePoints, ImVec2(244.0f, 24.0f));
    ImGui::End();

    if (playerLifePoints <= 0)
    {
        ImGui::SetNextWindowPos(ImVec2(size.x /2, size.y /2));
        ImGui::SetNextWindowSize(ImVec2(298,96));
        ImGui::Begin("End Screen", nullptr, MAIN_WINDOW);
        ImGui::Text("You Died");
        ImGui::End();
    }
    if (enemyLifePoints <= 0)
    {
        ImGui::SetNextWindowPos(ImVec2(size.x /2, size.y /2));
        ImGui::SetNextWindowSize(ImVec2(298,96));
        ImGui::Begin("End Screen", nullptr, MAIN_WINDOW);
        ImGui::Text("Victory");
        ImGui::End();
    }
    // Render ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
