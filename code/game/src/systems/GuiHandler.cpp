//
// Created by Julian Kalb on 05/07/2025.
//

#include "GuiHandler.h"

#include <iostream>
#include <stb_image.h>

#include "../gui/GuiCombat.h"


namespace gl3 {
    GuiHandler::GuiHandler(engine::Game& game):
    System(game),
    window(game.getWindow())
    {
        engine.onAfterStartup.addListener([&](engine::Game &game)
        {
            initialize(game);
            selectCurrentScene(game);
        });
        engine.onBeforeUpdate.addListener([&](engine::Game &game)
        {
            nk_glfw3_new_frame(&glfw);
        });
        engine.onAfterUpdate.addListener([&](engine::Game &game)
        {
            renderGUI(game);

        });
    }

    GuiHandler::~GuiHandler()
    {
        nk_glfw3_shutdown(&glfw);
    }

    void GuiHandler::initialize(engine::Game &game)
    {
        nkCTX = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
        {
            nk_font_atlas *atlas;
            nk_glfw3_font_stash_begin(&glfw, &atlas);
            nk_font *FantasyRPG1 = nk_font_atlas_add_from_file(atlas, "assets/textures/gui/FantasyRPG1.ttf", 20, 0);
            nk_glfw3_font_stash_end(&glfw);
            /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
            nk_style_set_font(nkCTX, &FantasyRPG1->handle);
        }
        loadTextureAtlas("assets/textures/gui/ui_atlas_48x48.png");

        combatGUI = std::make_unique<GuiCombat>(game, nkCTX, textureAtlasID);
    }

    void GuiHandler::selectCurrentScene(engine::Game& game)
    {

    }

    void GuiHandler::renderGUI(engine::Game& game)
    {
        int windowWidth, windowHeight;
        glfwGetWindowSize(game.getWindow(), &windowWidth, &windowHeight);

        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON,
            MAX_VERTEX_BUFFER,
            MAX_ELEMENT_BUFFER);
        combatGUI->renderGUI(windowWidth, windowHeight);
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            // gui.resetSelection();
        }

    }


    void GuiHandler::loadTextureAtlas(const char* texturePath)
    {
        int width, height, channels;
        unsigned char* image = stbi_load(texturePath, &width, &height, &channels, 4); // Load as RGBA

        if (!image) {
            std::cerr << "Failed to load texture atlas: " << texturePath << "\n";
            return;
        }

        glGenTextures(1, &textureAtlasID);
        glBindTexture(GL_TEXTURE_2D, textureAtlasID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(image);
    }
} // gl3