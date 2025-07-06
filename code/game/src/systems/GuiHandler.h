//
// Created by Julian Kalb on 05/07/2025.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define NK_INCLUDE_STANDARD_IO
#include <nuklear_glfw_gl3.h>

#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "engine/ecs/System.h"
#include "engine/ecs/Gui.h"

struct GuiState;

namespace gl3 {

class GuiHandler: public engine::ecs::System {
public:
    GuiHandler(engine::Game &game );
    ~GuiHandler();
private:
    void initialize(engine::Game &game);
    void selectCurrentScene(engine::Game &game);
    void renderGUI(engine::Game &game);
    void loadTextureAtlas(const char* texturePath);
    //-----GUI Szenen-----
    GuiState *activeScene = nullptr;
    std::unique_ptr<engine::ecs::Gui> combatGUI = nullptr;

    GLuint textureAtlasID = -1;
    GLFWwindow* window;
    nk_context *nkCTX;
    nk_glfw glfw = {nullptr};

};

} // gl3