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
        virtual void renderGUI(int windowWidth, int windowHeigh) = 0;
    protected:
        Game &engine;
        nk_context *ctx;
        nk_uint textureID;


    };
}
