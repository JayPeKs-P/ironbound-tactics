//
// Created by Julian Kalb on 17/07/2025.
//

#pragma once
#include <iostream>
#include <filesystem>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include "engine/ecs/System.h"
#include "../../components/InstanceBuffer.h"

namespace fs = std::filesystem;

namespace gl3 {
class RenderSystem: public engine::ecs::System {
    friend class Game;
    explicit RenderSystem(engine::Game &game);
    void draw(engine::Game &game);
    void update(engine::Game& game);
    void initShaders(engine::Game &game);
    void initBuffers(engine::Game &game);
    void deleteBuffers(engine::Game& game);
    void deleteShader(engine::Game& game);
    static void setMatrix(const std::string &uniformName, glm::mat4 matrix, GLuint shaderProgram) ;
    static void setVector(const std::string &uniformName, glm::vec4 vector, GLuint shaderProgram) ;
    static void setFloat(const std::string &uniformName, float value, GLuint shaderProgram) ;
    unsigned int loadAndCompileShader(GLuint shaderType, const fs::path& shaderPath);
    unsigned int batchVertexShader;
    unsigned int batchFragmentShader;
    unsigned int batchProgram;
    unsigned int singleVertexShader;
    unsigned int singleFragmentShader;
    unsigned int singleProgram;

};

} // gl3