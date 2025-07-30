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

namespace gl3::engine::render {

    /// @defgroup Rendering Everything related to rendering objects.
    /// @ingroup CoreSystems
    /// @brief Handles OpenGL rendering of 2D models with support for instancing.
    ///
    /// The RenderSystem registers itself to the following events:
    /// - @ref gl3::engine::Game::onAfterStartup → initShaders() and initBuffers()
    /// - @ref gl3::engine::Game::onUpdate → update()
    /// _ @ref gl3::engine::Game::onShutdown → deleteBuffers() and deleteShaders()
    ///
    /// This system draws all entities with a Model2D and optionally InstanceBuffer component using
    /// either instanced or non-instanced rendering. All member functions and variables are set to
    /// private. 'Game' is registered as friend class.
class RenderSystem: public engine::ecs::System {
    friend class Game;

    /// @brief Create a RenderSystem object and register functions to core game events.
    ///
    /// @param game Reference to core game object, used to instantiate @ref gl3::engine::ecs::System.
    explicit RenderSystem(engine::Game &game);

    /// @brief Draws all render-able entities
    /// Called manually in engine::Game::run() after draw().
    /// @param game Reference to core Game object.
    void draw(engine::Game &game);

    /// Uploads instance buffer data (called each frame).
    /// @param game Reference to core Game object.
    void update(engine::Game& game);

    /// Loads shaders and links OpenGL programs.
    /// @param game Reference to core Game object.
    void initShaders(engine::Game &game);

    /// Generates VAOs and VBOs for each Model2D component.
    /// @param game Reference to core Game object.
    void initBuffers(engine::Game &game);

    /// Deletes OpenGL buffers (VAO, VBO, EBO).
    /// @param game Reference to core Game object.
    void deleteBuffers(engine::Game& game);

    /// Delete shader objects.
    /// @param game Reference to core Game object.
    void deleteShader(engine::Game& game);

    /// @brief Uploads a glm::mat4 to the given shader uniform.
    /// @param uniformName Reference to the name of the uniform variable to set in the shader as String
    /// @param matrix The value the specified uniform will be set to.
    /// @param shaderProgram The shader program to target.
    static void setMatrix(const std::string &uniformName, glm::mat4 matrix, GLuint shaderProgram) ;

    /// @brief Uploads a glm::vec4 to the given shader uniform.
    /// @param uniformName Reference to the name of the uniform variable to set in the shader as String
    /// @param vector The value the specified uniform will be set to.
    /// @param shaderProgram The shader program to target.
    static void setVector(const std::string &uniformName, glm::vec4 vector, GLuint shaderProgram) ;

    /// @brief Uploads a float to the given shader uniform.
    /// @param uniformName Reference to the name of the uniform variable to set in the shader as String
    /// @param value The value the specified uniform will be set to.
    /// @param shaderProgram The shader program to target.
    static void setFloat(const std::string &uniformName, float value, GLuint shaderProgram) ;

    /// @brief Compiles a GLSL shader from file
    /// @param shaderType The type of shader to compile. Accepts OpenGL macros.
    /// @param shaderPath The path, were the GLSL shader file is located.
    unsigned int loadAndCompileShader(GLuint shaderType, const fs::path& shaderPath);

    /// ID of the vertex shader used for instance rendering.
    unsigned int batchVertexShader;

    /// ID of the fragment shader used for instance rendering.
    unsigned int batchFragmentShader;

    /// ID of the shader program used for instance rendering.
    unsigned int batchProgram;

    /// ID of the vertex shader used for non-instance rendering.
    unsigned int singleVertexShader;

    /// ID of the fragment shader used for non-instance rendering.
    unsigned int singleFragmentShader;

    /// ID of the shader program used for non-instance rendering.
    unsigned int singleProgram;

};

} // gl3