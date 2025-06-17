#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Context.h"

namespace gl3::engine {
    class Game{
    public:
        virtual void run();
        static glm::mat4 calculateMvpMatrix(glm::vec3 position, float zRotationDegrees, glm::vec3 scale);
        GLFWwindow *getWindow() {return context.getWindow();}
    protected:
        Game(int width, int height, const std::string& title);
        virtual ~Game();

        virtual void start(){};
        virtual void update(GLFWwindow *window){};
        virtual void draw(){};
        virtual void updateDeltaTime();

        float deltaTime = 1.0f / 60;
    private:
        glm::mat4 mvpMatrix{};
        context::Context context;
        float lastFrameTime = 1.0f / 60;

      };
}