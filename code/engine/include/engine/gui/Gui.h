//
// Created by Julian Kalb on 19/08/2025.
//

#pragma once
#include <string>
#include "nuklear_glfw_gl3.h"


namespace gl3::engine::ui {
    /// @brief Abstract base class for GUI scenes that can be managed by GuiManager.
    class Gui {
    public:
        explicit Gui(const std::string& name) : sceneName(name) {}

        virtual ~Gui() = default;

        /// @brief Called every frame to update and render the GUI scene.
        ///
        /// @param ctx Pointer to the Nuklear context the GUI should appear in.
        virtual void update(nk_context* ctx) = 0;

        /// @brief Called when the scene becomes active.
        virtual void onActivate() {}

        /// @brief Called when the scene becomes inactive.
        virtual void onDeactivate() {}

        /// @brief Called when the window size is changed
        virtual void onWindowResize(int width, int height) {}

        const std::string& getSceneName() const { return sceneName; }

    private:
        std::string sceneName;
    };
}
