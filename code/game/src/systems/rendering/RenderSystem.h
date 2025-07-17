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
    struct InstanceData
    {
        glm::mat4 modelMatrix;
        // float uvOffset;
    };

class RenderSystem: public engine::ecs::System {
    explicit RenderSystem(engine::Game &game);
    ~RenderSystem() override;
    void draw(engine::Game &game);
    void update(InstanceBuffer instanceBuffer_C);
    void initShaders(engine::Game &game);
    void initBuffers(engine::Game &game);
    void deleteBuffers();
    void deleteShader();
    static void setMatrix(const std::string &uniformName, glm::mat4 matrix, GLuint shaderProgram) ;
    static void setVector(const std::string &uniformName, glm::vec4 vector, GLuint shaderProgram) ;
    static void setFloat(const std::string &uniformName, float value, GLuint shaderProgram) ;
    unsigned int loadAndCompileShader(GLuint shaderType, const fs::path& shaderPath);

};

} // gl3