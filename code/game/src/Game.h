//
// Created by julia on 22/10/2024.
//
#pragma once
#include <iostream>

//#include "../src/entities/Ship.h"
#include "../src/entities/Entity.h"


#include <soloud.h>
#include <soloud_wav.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
//
// #include "guiElements/BattleMenu.h"

#include <GLFW/glfw3.h>

// #define NK_INCLUDE_FIXED_TYPES
// #define NK_INCLUDE_STANDARD_IO
// #define NK_INCLUDE_STANDARD_VARARGS
// #define NK_INCLUDE_DEFAULT_ALLOCATOR
// #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
// #define NK_INCLUDE_FONT_BAKING
// #define NK_INCLUDE_DEFAULT_FONT
// #define NK_IMPLEMENTATION
// #define NK_GLFW_GL3_IMPLEMENTATION
// #define NK_KEYSTATE_BASED_INPUT
// #include <nuklear.h>
#include <nuklear_glfw_gl3.h>

#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024

/*#define INCLUDE_ALL */
/*#define INCLUDE_STYLE */
/*#define INCLUDE_CALCULATOR */
// /*#define INCLUDE_CANVAS */
// #define INCLUDE_OVERVIEW
/*#define INCLUDE_NODE_EDITOR */




namespace gl3
{
    class Game {
    public:
        Game(int width, int height, const std::string& title);
        ~Game();
        static glm::mat4 calculateMvpMatrix(glm::vec3 position, float zRotationDegrees, glm::vec3 scale);

        void run();

        [[nodiscard]] GLFWwindow* getWindow() const
        {
            return window;
        }
        // [[nodiscard]] Ship* getShip() const
        // {
        //     return ship;
        // }

    private:
        struct nk_glfw glfw = {nullptr};
        struct nk_context *ctx;
        struct nk_colorf bg;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        void update();
        void draw();
        void updateDeltaTime();

        std::vector<std::unique_ptr<Entity>> entities;

//        Ship* ship = nullptr;
        GLFWwindow* window = nullptr;
        glm::mat4 mvpMatrix{};

        // BattleMenu* battleMenu = nullptr;

        SoLoud::Soloud audio;
        std::unique_ptr<SoLoud::Wav>(backgroundMusic);

        float lastFrameTime{};
        float deltaTime{};

        float zRotation = 0.0f;
        float rotationSpeed = 120.0f;
        float xTranslate = 0.0f;
        float yTranslate = 0.0f;
        float translationSpeed = 1.0f;



    };
}
