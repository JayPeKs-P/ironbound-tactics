//
// Created by julia on 27/11/2024.
//
#include <iostream>
#include "battleMenu.h"

using namespace gl3;

void battleMenu::renderBattleMenu()
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

    ImGui::End();

    // Render ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
