//
// Created by julia on 19/01/2025.
//

#pragma once
#include <nuklear_glfw_gl3.h>
//-----------------------------------------
/*
NOTE TO MYSELF: Shold there ever be any
weird behavior with rendering, I never
fully understood those macros (they are
used in nk_glfw3_render(), which uses their
data for glBufferData()
*/
#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024
//-----------------------------------------

namespace gl3 {

class GUI {
    public:
    explicit GUI(GLFWwindow *window);
    ~GUI();

    void update();
    void draw();

    private:
    struct nk_glfw glfw = {nullptr};
    struct nk_context *ctx;

};

} // gl3