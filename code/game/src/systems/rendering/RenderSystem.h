//
// Created by Julian Kalb on 17/07/2025.
//

#pragma once
#include <iostream>
#include <filesystem>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include "engine/ecs/System.h"

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
    void update();
    void initBuffers(engine::Game &game);
    void deleteBuffers();
    void deleteShader();
    void loadAndCompileShader(GLuint shaderType, const fs::path &shaderPath);

};

} // gl3