//
// Created by Julian Kalb on 06/07/2025.
//

#pragma once
#include "engine/Game.h"
#include <nuklear_glfw_gl3.h>

namespace gl3::engine::ecs
{
    class Gui {
    public:
        explicit Gui(Game &engine, nk_context* ctx, nk_uint textureID)
            : engine(engine), ctx(ctx), textureID(textureID) {}
        Gui(Gui &&) = delete;
        Gui(const Gui &) = delete;
        virtual ~Gui() = default;
        virtual void renderGUI() = 0;
        void updateMargins(int newWidth, int newHeight){windowWidth = newWidth; windowHeight = newHeight;};
    protected:
        int windowWidth, windowHeight;
        Game &engine;
        nk_context *ctx;
        nk_uint textureID;


    };
}
