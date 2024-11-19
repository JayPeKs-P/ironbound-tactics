//
// Created by julia on 23/10/2024.
//

#pragma once

#include <iostream>
#include <filesystem>

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace fs = std::filesystem;

namespace gl3 {

    class Shader {
        public:
        Shader(const fs::path &vertexShaderSource, const fs::path &fragmentShaderSource);
        ~Shader();
        void use() const;
        void setMatrix(const std::string &uniformName, glm::mat4 matrix) const;
        void setVector(const std::string &uniformName, glm::vec4 vector) const;

        Shader(const Shader &shader) = delete;

        // Explicit move constructor
        Shader(Shader &&other) noexcept {
            std::swap(this->shaderProgram, other.shaderProgram);
            std::swap(this->vertexShader, other.vertexShader);
            std::swap(this->fragmentShader, other.fragmentShader);
        }
        private:
        unsigned int shaderProgram;
        unsigned int vertexShader;
        unsigned int fragmentShader;

        static unsigned int loadAndCompileShader(GLuint shaderType, const fs::path& shaderPath);


    };

} // gl3