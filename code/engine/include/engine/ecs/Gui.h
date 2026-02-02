//
// Created by Julian Kalb on 06/07/2025.
//

#pragma once
#include "engine/Game.h"
// #include <nuklear_glfw_gl3.h>
#include "NuklearWrapper.h"

/// @defgroup GUI GUI scenes
/// All GUI scenes that can be displayed in GuiHandler. All GUI scenes use the Nuklear library for implementation.
/// @ingroup Engine
namespace gl3::engine::ecs
{
    /// @ingroup GUI
    /// @brief Abstract base class for all GUI scenes.
    ///
    /// A GUI scene has access to the ECS context (engine::Game object).
    ///
    /// All GUI scenes should inherit from this class.
    class Gui {
    public:
        /// @brief Constructs a GUI object and stores reference to the game instance, a pointer to the Nuklear context
        /// (nk_context) and the ID of the texture Atlas used to style the GUI.
        /// @param engine Reference to core game object.
        /// @param ctx Pointer to Nuklear context object.
        /// @param textureID ID of the texture, loaded with OpenGL
        explicit Gui(Game &engine, nk_context* ctx, nk_uint textureID)
            : engine(engine), ctx(ctx), textureID(textureID) {}

        /// Prevent move semantics.
        Gui(Gui &&) = delete;

        /// Delete copy constructor.
        Gui(const Gui &) = delete;

        virtual ~Gui() = default;

        /// @brief Public function, calls render() if endScene variable is and false invokeSceneChange() else.
        /// This function is called manually in GuiHandler.
        void update()
        {
            if (!endScene) render();
            else
            {
                invokeSceneChange();
                endScene = false;
            }
        }

        /// @brief Updates the stored values of width and height, should the window change its size.
        /// @param newWidth New width of window.
        /// @param newHeight New height of window.
        void updateMargins(int newWidth, int newHeight){windowWidth = newWidth; windowHeight = newHeight;};
    protected:

        /// @brief Function to check, if one or more entities have the specified component.
        ///
        /// This utility function returns true, if all entities, whose IDs are provided, have a Component of type T.
        /// @tparam  T Component type to check.
        /// @tparam Guids List of guid_t values of the entities to check.
        /// @return true if all provided entities have component T, false otherwise.
        template <typename T, typename... Guids>
        bool checkIfEntityHasComponent(Guids... guids)
        {
            return (... && engine.componentManager.hasComponent<T>((guids)));
        }

        /// @brief Function is called in update() if endScene is true. Can be used to invoke an event.
        virtual void invokeSceneChange() = 0;

        /// @brief Function is called in update().
        /// Always overwrite this function in child class. Use this function to create nk_begin and nk_end.
        virtual void render() = 0;

        /// Store the width and height of the GLFWwindow. Used to calculate relative size of Nuklear objects.
        int windowWidth, windowHeight;
        bool endScene = false;
        std::unordered_map<std::string, bool> m_Hovered;
        Game &engine;
        nk_context *ctx;
        nk_uint textureID;
    };
}
