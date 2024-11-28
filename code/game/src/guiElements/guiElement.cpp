//
// Created by julia on 26/11/2024.
//

#include "guiElement.h"
#include "../Game.h"

using namespace gl3;

guiElement::~guiElement()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void guiElement::initializeGUI(Game* Game)
{
    auto window = Game->getWindow();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // Capture user input and configurations
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark(); // Set the default ImGui style

}
