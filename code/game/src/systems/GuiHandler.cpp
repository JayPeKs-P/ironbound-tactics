//
// Created by Julian Kalb on 05/07/2025.
//

#include "GuiHandler.h"
#include "engine/Texture.h"
#include <iostream>


#include "../components/GuiState.h"
#include "../gui/GuiUnitSelection.h"
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
            restoreGameState();
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
        textureAtlasID = engine::util::Texture::load("assets/textures/gui/ui_atlas_48x48.png", false);
        nk_style* style = &nkCTX->style;
        setStyleWindow(style);
        setStyleButton(style);
        setStyleOptionLabel(style);
        setStyleProgressBar(style);
        setStyleSlider(style);
        setStyleText(style);

        // combatGUI = std::make_unique<GuiUnitSelection>(game, nkCTX, textureAtlasID);
        combatGUI = std::make_unique<GuiCombat>(game, nkCTX, textureAtlasID);
        for (auto& [owner, _] : game.componentManager.getContainer<GuiState>())
        {
            if (game.componentManager.hasComponent<GuiState>(owner))
            {
                activeScene = &game.componentManager.getComponent<GuiState>(owner);
            }
        }
    }

    void GuiHandler::restoreGameState()
    {
        glDisable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //
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
                combatGUI->updateMargins(windowWidth, windowHeight);
                combatGUI->renderGUI();
                break;
            case GuiScene::UNIT_SELECTION:
                break;
        }
    }
    void GuiHandler::setStyleWindow(nk_style* style)
    {
        // style->window.fixed_background = nk_style_item_image(getTileImage(2, 54, 1, 1, 3072, 3072));
        style->window.fixed_background = nk_style_item_color(nk_rgba(0,0,0,120));
        style->window.border = 10.0f;
        style->window.border_color = nk_rgba(120, 58, 58, 163);
        style->window.header.label_normal = nk_rgba(255, 250, 200, 255);
        style->window.header.label_hover = nk_rgba(255, 250, 200, 255);
        style->window.header.label_active = nk_rgba(255, 250, 200, 255);
        style->window.header.normal = nk_style_item_color(nk_rgba(0,0,0,200));
        style->window.header.hover = nk_style_item_color(nk_rgba(0,0,0,200));
        style->window.header.active = nk_style_item_color(nk_rgba(0,0,0,200));
        // style->window.header.normal = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
        // style->window.header.hover = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
        // style->window.header.active = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
    }

    void GuiHandler::setStyleButton(nk_style* style)
    {
        style->button.normal = nk_style_item_image(getTileImage(0, 11, 3, 1, 3072, 3072));
        style->button.hover = nk_style_item_image(getTileImage(3, 11, 3, 1, 3072, 3072));
        style->button.active = nk_style_item_image(getTileImage(6, 11, 3, 1, 3072, 3072));
        style->button.text_normal = nk_rgba(255, 255, 255, 255);
        style->button.text_hover = nk_rgba(255, 250, 200, 255);
    }

    void GuiHandler::setStyleOptionLabel(nk_style* style)
    {
        style->option.normal = nk_style_item_image(getTileImage(21, 11, 1, 1, 3072, 3072));
        style->option.hover = nk_style_item_image(getTileImage(22, 11, 1, 1, 3072, 3072));
        style->option.active = nk_style_item_image(getTileImage(23, 11, 1, 1, 3072, 3072));
        style->option.cursor_normal = nk_style_item_image(getTileImage(24, 11, 1, 1, 3072, 3072));
        style->option.cursor_hover = nk_style_item_image(getTileImage(25, 11, 1, 1, 3072, 3072));
        style->option.text_normal = nk_rgba(255, 255, 255, 255);
        style->option.text_hover = nk_rgba(255, 250, 200, 255);
    }

    void GuiHandler::setStyleProgressBar(nk_style* style)
    {
        style->progress.normal = nk_style_item_image(getTileImage(0, 14, 3, 1, 3072, 3072));
        style->progress.cursor_normal = nk_style_item_image(getTileImage(6, 14, 3, 1, 3072, 3072));
    }

    void GuiHandler::setStyleSlider(nk_style* style)
    {
        style->slider.normal = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
        style->slider.hover = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
        style->slider.active = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
        style->slider.cursor_normal = nk_style_item_image(getTileImage(21, 12, 1, 1, 3072, 3072));
        style->slider.cursor_hover = nk_style_item_image(getTileImage(22, 12, 1, 1, 3072, 3072));
        style->slider.cursor_active = nk_style_item_image(getTileImage(23, 12, 1, 1, 3072, 3072));
        style->slider.bar_filled = nk_rgba(100, 100, 200, 200);
        style->slider.bar_normal = nk_rgba(92, 58, 58, 163);
        style->slider.bar_hover = nk_rgba(92, 58, 58, 163);
        style->slider.bar_active = nk_rgba(92, 58, 58, 163);
    }

    void GuiHandler::setStyleText(nk_style* style)
    {
        style->text.color = nk_rgba(255, 255, 255, 255);
    }

    struct nk_image GuiHandler::getTileImage(int tileX, int tileY, int tileWidth, int tileHeight, int atlasWidth, int atlasHeight)
    {
        return nk_subimage_id(textureAtlasID, atlasWidth, atlasHeight,
                              nk_rect(tileX * 48, tileY * 48, tileWidth * 48, tileHeight * 48));
    }
} // gl3