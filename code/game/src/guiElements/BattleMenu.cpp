//
// Created by julia on 27/11/2024.
//

#include <iostream>
#include "BattleMenu.h"

#include <nuklear_glfw_gl3.h>


using namespace gl3;

BattleMenu::BattleMenu(struct nk_vec2& size):
    GuiElement(PngTexture("textures/gui/ui_atlas_48x48.png"), 48, 48),
    size(size),
    playerArmy(40, 11, 3, glm::vec3(2.0f, 0.0f, 0.0f)),
    enemyArmy(36, 12, 4, glm::vec3(-2.0f, 0.0f, 0.0f))
{

}

void BattleMenu::renderBattleMenu()
{
    auto MAIN_WINDOW = NK_WINDOW_NO_INPUT;

    // // Begin Nuklear frame
    nk_glfw3_new_frame(&glfw);

    // Create a simple window
    if (nk_begin(ctx, "Combat Menu", nk_rect(0, size.y - size.y / 3, size.x, size.y / 3),
                MAIN_WINDOW)
    {
        nk_layout_row_static(ctx, 48, 144, 3);
        nk_label(ctx

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
    if (ImGui::Button("Ready", ImVec2(144, 48)))
    {
        if (!playersTurn)
        {
            playerArmy.takeDamage(enemyArmy.dealDamage());
            // <- dealDamage() resets all units to ready -> when put in one scope reset to early
            playerLifePoints = playerArmy.getArmySize() / playerArmy.maxArmySize;
        }
        else
        {
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
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.00f, 0.00f, 1.00f));
        ImGui::Text("\tEnemies Turn");
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("Defend", ImVec2(144, 48)))
    {
        if (playersTurn)
        {
            playerArmy.setDefending(UnitHumanoid::Type::Infantry, infantrySelected);
            infantrySelected = 0;
            playerArmy.setDefending(UnitHumanoid::Type::Archer, archersSelected);
            archersSelected = 0;
            //playerArmy.setDefending(UnitHumanoid::Type::Siege, siegeSelected);
            siegeSelected = 0;
        }
        else
        {
            enemyArmy.setDefending(UnitHumanoid::Type::Infantry, infantrySelected);
            infantrySelected = 0;
            enemyArmy.setDefending(UnitHumanoid::Type::Archer, archersSelected);
            archersSelected = 0;
            //enemyArmy.setDefending(UnitHumanoid::Type::Siege, siegeSelected);
            siegeSelected = 0;
        }
    }
    ImGui::PopStyleVar();

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(size.x / 6, size.y - 192));
    ImGui::SetNextWindowSize(ImVec2(336, 192));
    ImGui::Begin("UnitButtons", nullptr, BATTLE_MENU);

    ImGui::Text("Infantry \nfor defense");
    ImGui::SameLine();
    if (ImGui::Button("+##infantry", ImVec2(48, 48)))
    {
        infantrySelected++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-##infantry", ImVec2(48, 48)) && infantrySelected > 0)
    {
        infantrySelected--;
    }
    ImGui::SameLine();
    ImGui::Text("Amount: \n%d", infantrySelected);
    ImGui::Text("Archers \nfor defense");
    ImGui::SameLine();
    if (ImGui::Button("+##archer", ImVec2(48, 48)))
    {
        archersSelected++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-##archer", ImVec2(48, 48)) && archersSelected > 0)
    {
        archersSelected--;
    }
    ImGui::SameLine();
    ImGui::Text("Amount: \n%d", archersSelected);
    ImGui::Text("Siege\nfor defense");
    ImGui::SameLine();
    if (ImGui::Button("+##siege", ImVec2(48, 48)))
    {
        siegeSelected++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-##siege", ImVec2(48, 48)) && siegeSelected > 0)
    {
        siegeSelected--;
    }
    ImGui::SameLine();
    ImGui::Text("Amount: \n%d", siegeSelected);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(size.x / 2, size.y - 192));
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
        ImGui::SetNextWindowPos(ImVec2(size.x / 2 - 48, size.y / 2 - 8));
        ImGui::SetNextWindowSize(ImVec2(298, 96));
        ImGui::Begin("End Screen", nullptr, MAIN_WINDOW);
        ImGui::Text("You Died");
        ImGui::End();
    }
    if (enemyLifePoints <= 0 && !(playerLifePoints <= 0))
    {
        ImGui::SetNextWindowPos(ImVec2(size.x / 2, size.y / 2));
        ImGui::SetNextWindowSize(ImVec2(298, 96));
        ImGui::Begin("End Screen", nullptr, MAIN_WINDOW);
        ImGui::Text("Victory");
        ImGui::End();
    }

    // Render ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void BattleMenu::draw(Game* game)
{
    playerArmy.draw(game);
    enemyArmy.draw(game);
}
