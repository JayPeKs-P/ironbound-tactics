//
// Created by Julian Kalb on 19/08/2025.
//

#include "engine/gui/GuiManager.h"
#include "engine/Game.h"
#include "engine/Log.h"
#include "engine/util/Assets.h"

namespace gl3::engine::ui {
    GuiManager::GuiManager(Game& game) :
    System(game),
    window(game.getWindow()){
        engine.onAfterStartup.addListener([&](Game&)
        {
            initialize();
        });

        engine.onUpdate.addListener([&](Game&)
        {
            update();
        });

        engine.onAfterUpdate.addListener([&](Game&)
        {
            render();
            restoreOpenGLState();
        });

        engine.onShutdown.addListener([&](Game&)
        {
            cleanup();
        });
    }

    GuiManager::~GuiManager() {
        cleanup();
    }

    void GuiManager::switchToScene(const std::string& sceneName) {
        CORE_TRACE("Switching to Gui scene [{}]", sceneName);
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
        {
            CORE_ERROR("Could not swap to scene [{}] not registered in container.", sceneName);
            throw std::runtime_error("GuiManager: Scene [" + sceneName + "] not found");
        }
        if (currentScene != nullptr)
        {
            currentScene->onDeactivate();
        }

        activeSceneName = sceneName;
        currentScene = it->second.get();
        currentScene->onActivate();

        onSceneChangeRequested.invoke(sceneName);
    }

    void GuiManager::initialize() {
        glfwGetWindowSize( window, &windowWidth, &windowHeight);
        lastWindowHeight = windowHeight;

        nkCTX = nk_glfw3_init(&nuklearGLFW, window, NK_GLFW3_INSTALL_CALLBACKS);
        if (!nkCTX) {
            throw std::runtime_error("GuiManager: Failed to initialize Nuklear GUI context");
        }

        {
            nk_glfw3_font_stash_begin(&nuklearGLFW, &fontAtlas);
            setupFonts(fontAtlas, windowHeight);
            nk_glfw3_font_stash_end(&nuklearGLFW);
        }
        setupStyling(&nkCTX->style);
    }

    void GuiManager::update() {
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        if (windowHeight != lastWindowHeight)
        {
            lastWindowHeight = windowHeight;

            nk_glfw3_font_stash_begin(&nuklearGLFW, &fontAtlas);
            setupFonts(fontAtlas, windowHeight);
            nk_glfw3_font_stash_end(&nuklearGLFW);

            if (currentScene != nullptr)
            {
                currentScene->onWindowResize(windowWidth, windowHeight);
            }
        }

        nk_glfw3_new_frame(&nuklearGLFW);

        if (currentScene != nullptr)
        {
            currentScene->update(nkCTX);
        }
    }

    void GuiManager::render() {
        nk_glfw3_render(&nuklearGLFW, NK_ANTI_ALIASING_ON,
            MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    }

    void GuiManager::cleanup() {
        if (nkCTX)
        {
            nk_glfw3_shutdown(&nuklearGLFW);
            nkCTX = nullptr;
        }
    }

    void GuiManager::restoreOpenGLState() {
        glDisable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}
