//
// Created by julia on 25/02/2025.
//

#pragma once

#include <GLFW/glfw3.h>
#define NK_INCLUDE_STANDARD_IO
#include <nuklear_glfw_gl3.h>

namespace gui {

class GuiContainer {
    public:
    GuiContainer(GLFWwindow* window);
    ~GuiContainer();

    void init();
    void draw();
    void update();

    private:
    struct nk_glfw glfw = {nullptr};
    struct nk_context *ctx;

    GLFWwindow* window;

};

} // gui