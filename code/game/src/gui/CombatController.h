//
// Created by julia on 25/02/2025.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_STANDARD_IO
#include <nuklear_glfw_gl3.h>

#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "GuiCombat.h"
#include "../combatLogic/Army.h"

namespace gui {

class CombatController {
    public:
    CombatController(GLFWwindow* window);
    ~CombatController();

    void init();
    void draw();
    void update();

    private:
    // GuiCombat guiCombat;
    gl3::Army* playerArmy = nullptr;
    gl3::Army* aiArmy = nullptr;

    GLuint textureAtlasID = -1;
    void loadTextureAtlas(const char* texturePath);
    struct nk_glfw glfw = {nullptr};
    struct nk_context *ctx;

    GLFWwindow* window;

};

} // gui