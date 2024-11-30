//
// Created by julia on 27/11/2024.
//
#include <iostream>
#include "battleMenu.h"


using namespace gl3;

BattleMenu::BattleMenu():
guiElement(PngTexture("textures/gui/ui_atlas_48x48.png"))
{

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.9f, 0.1f, 0.0f)); // Button color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.1f, 1.0f)); // Hover color
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.1f, 1.0f)); // Active color

    /*ImGui::PopStyleColor(3); // Revert the styling*/
}

void BattleMenu::renderBattleMenu()
{
    // Begin ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a simple window
    ImGui::Begin("Battle Menu");

    // Battle menu options
    if (ImGui::Button("Fight")) {
        std::cout << "Fight selected!" << std::endl;
    }
    if (ImGui::Button("Bag")) {
        std::cout << "Bag selected!" << std::endl;
    }
    if (ImGui::Button("Run")) {
        std::cout << "Run selected!" << std::endl;
    }
    if (ImGui::Button("Pokemon")) {
        std::cout << "Pokemon selected!" << std::endl;
    }

    // Define UV coordinates for a specific subregion
    ImVec2 uv0 = ImVec2(0.0f, 0.0f); // Top-left
    ImVec2 uv1 = ImVec2(90.0f, 90.0f); // Bottom-right (subregion)

    // Render button with texture
    ImGui::ImageButton("testTextureButton", textureID, ImVec2(96, 48), uv0, uv1);

    ImGui::End();

    // Render ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
