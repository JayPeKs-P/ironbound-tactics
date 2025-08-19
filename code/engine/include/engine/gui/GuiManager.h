//
// Created by Julian Kalb on 19/08/2025.
//

#pragma once
#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Gui.h"
#include "engine/ecs/System.h"
#include "engine/Events.h"

#define NK_INCLUDE_STANDARD_IO
#include "nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024

namespace gl3::engine::ui {

    using guiEvent_t = events::Event<class GuiManager, const std::string&>;
    /// @brief
class GuiManager : public ecs::System {
    explicit GuiManager(Game &game);
    virtual ~GuiManager();

    template<typename T, typename ...Args>
    void registerScene(const std::string &sceneName, Args&&... args) {
        static_assert(std::is_base_of_v<Gui, T>, "T must be a subclass of Gui");
        scenes[sceneName] = std::make_unique<T>(sceneName, std::forward<Args>(args)...);
    }

    void switchToScene(const std::string &sceneName);

    Gui *getCurrentScene() const { return currentScene; }
    nk_context *getNuklearContext() const { return nkCTX; }

    //TODO: ADD loadTexture() and createTileImage()?

    guiEvent_t onSceneChangeRequested;

protected:
    virtual void setupStyling(nk_style *style) {}
    virtual void setupFonts(nk_font_atlas *atlas, int windowHeight) {}

private:
    void initialize();
    void update();
    void render();
    void cleanup();
    void restoreOpenGLState();

    GLFWwindow *window;
    nk_context *nkCTX = nullptr;
    nk_glfw nuklearGLFW = {nullptr};
    nk_font_atlas *fontAtlas = nullptr;

    std::unordered_map<std::string, std::shared_ptr<Gui>> scenes;
    Gui *currentScene = nullptr;
    std::string activeSceneName;

    int windowWidth, windowHeight = 0;
    int lastWindowHeight = 720;
};

}
