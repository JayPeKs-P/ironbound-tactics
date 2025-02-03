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
#include <nuklear_glfw_gl3.h>
//-----------------------------------------
/*
NOTE TO MYSELF: Shold there ever be any
weird behavior with rendering, I never
fully understood those macros (they are
used in nk_glfw3_render(), which uses their
data for glBufferData()
*/
#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024
//-----------------------------------------


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
        void initGUI();
        void updateGUI();
        void drawGUI();
        void destroyGUI();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        void update();
        void draw();
        void updateDeltaTime();

        struct nk_glfw glfw = {nullptr};
        struct nk_context *ctx;

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
