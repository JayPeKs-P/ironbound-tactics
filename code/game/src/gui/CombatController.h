//
// Created by Julian Kalb on 25/02/2025.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_STANDARD_IO
#include <nuklear_glfw_gl3.h>

#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "engine/ecs/System.h"
#include "engine/Events.h"

#include "GuiCombat.h"
#include "../entities/Unit.h"

namespace gl3 {

class CombatController: public engine::ecs::System {
    public:
    // TODO: check if this typedef needs to add <..., CombatController&> like in wp3 engine/game.h
    using eventCC_t = engine::events::Event<CombatController>;
    CombatController(engine::Game &game );
    ~CombatController();

    // events for combat and turn structure
    eventCC_t onStartCombat;
    eventCC_t onBeforeTurn;
    eventCC_t onAfterTurn;
    eventCC_t onEndCombat;

    void handleTurn(bool newRound);

    void init();
    void draw();
    void update();

    private:
    gui::GuiCombat* guiCombat = nullptr;

    GLuint textureAtlasID = -1;
    void loadTextureAtlas(const char* texturePath);
    struct nk_glfw glfw = {nullptr};
    struct nk_context *ctx;

    GLFWwindow* window;

};

} // gui