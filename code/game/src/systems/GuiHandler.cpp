//
// Created by Julian Kalb on 05/07/2025.
//

#include "GuiHandler.h"
#include "engine/Texture.h"
#include <iostream>


#include "../components/GuiState.h"
#include "../gui/GuiCombat.h"


namespace gl3 {
    GuiHandler::GuiHandler(engine::Game& game):
    System(game),
    window(game.getWindow())
    {
        engine.onAfterStartup.addListener([&](engine::Game &game)
        {
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
        glfwGetWindowSize(game.getWindow(), &windowWidth, &windowHeight);
        nkCTX = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
        {
            nk_glfw3_font_stash_begin(&glfw, &atlas);
            nk_font *FantasyRPG1 = nk_font_atlas_add_from_file(atlas, "assets/textures/gui/FantasyRPG1.ttf", windowHeight/36, 0);
            nk_glfw3_font_stash_end(&glfw);
            /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
            nk_style_set_font(nkCTX, &FantasyRPG1->handle);
        }
        textureAtlasID = engine::util::Texture::load("assets/textures/gui/ui_atlas_48x48.png");

        combatGUI = std::make_unique<GuiCombat>(game, nkCTX, textureAtlasID);
        for (auto& [owner, _] : game.componentManager.getContainer<GuiState>())
        {
            if (game.componentManager.hasComponent<GuiState>(owner))
            {
                activeScene = &game.componentManager.getComponent<GuiState>(owner);
            }
        }
    }

    void GuiHandler::selectCurrentScene(engine::Game& game)
    {

    }

    void GuiHandler::renderGUI(engine::Game& game)
    {
        glfwGetWindowSize(game.getWindow(), &windowWidth, &windowHeight);
        if (currentH != windowHeight)
        {
            currentH = windowHeight;
            nk_glfw3_font_stash_begin(&glfw, &atlas);
            int fontSize = std::max(windowHeight / 36, 10);
            nk_font *rpg1= nk_font_atlas_add_from_file(atlas, "assets/textures/gui/FantasyRPG1.ttf", fontSize, 0);
            nk_glfw3_font_stash_end(&glfw);
            if (rpg1)
            {
            nk_style_set_font(nkCTX, &rpg1->handle);
            }
            else
            {
                std::cerr << "Font konnte nicht geladen werden! (Größe: " << fontSize;
            }
        }
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON,
            MAX_VERTEX_BUFFER,
            MAX_ELEMENT_BUFFER);
        switch (activeScene->current)
        {
            case GuiScene::MAIN_MENU:
                break;
            case GuiScene::COMBAT_MENU:
                combatGUI->renderGUI(windowWidth, windowHeight);
                break;
            case GuiScene::UNIT_SELECTION:
                break;
        }
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            // gui.resetSelection();
        }
    }
} // gl3