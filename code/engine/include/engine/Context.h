//
// Created by Julian Kalb on 17/06/2025.
//

#pragma once
#include <string>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gl3::engine::context {
    struct WindowedData {
        int iWindowedX = 0;
        int iWindowedY = 0;
        int iWindowedWidth = 0;
        int iWindowedHeight = 0;
    };
    /// @brief The core class, that initializes the GLFW library, manages window creation, termination and runs the update loop.
    ///
    /// This class initializes the GLFW library and GLAD, creates the window and runs the update loop.
    /// It manages the lifetime of the window a game is displayed in.
    class Context {
    public:
        using Callback = std::function<void(Context&)>;

        /// @brief Handles the initialization and termination of the GLFWwindow.
        ///
        /// @param width Width of the window, default value is 800.
        /// @param height Height of the window, default value is 600.
        /// @param title The text, that is displayed in the border of the window, default value is "Game".
        explicit Context(int width = 800, int height = 600, const std::string &title = "Game");
        virtual ~Context();

        /// @brief Runs the update loop of the window
        ///
        /// This function runs the update loop of the window. It sets the clear color of the window, calls
        /// glfwPollEvents and glfwSwapBuffers.
        /// @param update A callable object with return type void. Invoked in every iteration of the update loop.
        void run(const Callback& update);
        [[nodiscard]] GLFWwindow *getWindow() { return window; }

    private:
        /// @brief Returns the GLFW window instance.
        ///
        /// @return Pointer to GLFWwindow.
        GLFWwindow *window = nullptr;
    };
}
