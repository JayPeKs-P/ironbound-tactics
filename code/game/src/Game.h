//
// Created by julia on 22/10/2024.
//
#pragma once
#include <iostream>

#include "../src/entities/Ship.h"


#include <soloud.h>
#include <soloud_wav.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "guiElements/battleMenu.h"


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
        [[nodiscard]] Ship* getShip() const
        {
            return ship;
        }

    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        void update();
        void draw();
        void updateDeltaTime();

        std::vector<std::unique_ptr<Entity>> entities;

        Ship* ship = nullptr;
        GLFWwindow* window = nullptr;
        glm::mat4 mvpMatrix{};

        battleMenu battleMenu;

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
