//
// Created by julia on 19/01/2025.
//

#pragma once
#include <nuklear_glfw_gl3.h>

namespace gl3 {

class GUI {
    public:
    GUI(struct nk_context *ctx,);

    void update();
    void draw();

};

} // gl3