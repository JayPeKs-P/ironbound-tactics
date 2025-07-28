//
// Created by Julian Kalb on 17/06/2025.
//

#include <stdexcept>
#include "engine/Context.h"

namespace gl3::engine::context {
    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    Context::Context(int width, int height, const std::string &title) {
        if(!glfwInit()) {
            throw std::runtime_error("Failed to initialize glfw");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if(window == nullptr) {
            throw std::runtime_error("Failed to create window");
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        glEnable(GL_DEPTH_TEST);
        if(glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("gl error");
        }
    }

    void Context::run(const Context::Callback& update) {
        glfwSetTime(1.0 / 60);
        while(!glfwWindowShouldClose(window)) {
            glClearColor(0.172f, 0.243f, 0.313f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            update(*this);
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    Context::~Context() {
        glfwTerminate();
    }
}