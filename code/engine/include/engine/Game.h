#pragma once


#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace gl3::engine {
    class Game{
    public:
        static glm::mat4 calculateMvpMatrix(glm::vec3 position, float zRotationDegrees, glm::vec3 scale);


        [[nodiscard]] GLFWwindow* getWindow() const
        {
            return window;
        }
    protected:
        Game(int width, int height, const std::string& title);
        virtual ~Game();

        virtual void run(){};
        virtual void update(){};
        virtual void draw(){};
        virtual void updateDeltaTime();

        GLFWwindow* window = nullptr;
        float deltaTime = 1.0f / 60;
    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        glm::mat4 mvpMatrix{};
        float lastFrameTime = 1.0f / 60;

      };
}