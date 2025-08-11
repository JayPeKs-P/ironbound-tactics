//
// Created by Julian Kalb on 17/07/2025.
//

#include "engine/rendering/RenderSystem.h"
#include <fstream>
#include <sstream>
#include "engine/util/Assets.h"

#include "engine/rendering/Model2D.h"
#include "engine/rendering/Shader.h"
#include "engine/rendering/InstanceBuffer.h"

#define MAX_INSTANCES 60

namespace gl3::engine::render {
    struct glStatusData {
        int success;
        const char* shaderName;
        char infoLog[GL_INFO_LOG_LENGTH];
    };

    std::string readText(const fs::path& filePath) {
        std::ifstream sourceFile(resolveAssetPath(filePath));
        std::stringstream buffer;
        buffer << sourceFile.rdbuf();
        return buffer.str();
    }

    template <typename T>
    GLuint createBuffer(GLuint bufferType, const std::vector<T>& bufferData) {
        unsigned int buffer = 0;
        glGenBuffers(1, &buffer);
        glBindBuffer(bufferType, buffer);
        glBufferData(bufferType, bufferData.size() * sizeof(T), bufferData.data(), GL_DYNAMIC_DRAW);
        return buffer;
    }

    RenderSystem::RenderSystem(engine::Game& game):
        System(game) {
        engine.onAfterStartup.addListener([&](engine::Game& game)
        {
            initShaders(game);
            initBuffers(game);
        });
        engine.onUpdate.addListener([&](engine::Game& game)
        {
            update(game);
        });
        engine.onShutdown.addListener([&](engine::Game& game)
        {
            deleteBuffers(game);
            deleteShader(game);
        });
    }

    void RenderSystem::draw(engine::Game& game) {
        auto& model2DContainer = game.componentManager.getContainer<Model2D>();
        for (auto& [owner, _] : model2DContainer)
        {
            auto& shader_C = game.componentManager.getComponent<Shader>(owner);
            glUseProgram(shader_C.get_shader_program());

            auto& model2D_C = game.componentManager.getComponent<Model2D>(owner);
            glBindVertexArray(model2D_C.VAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, model2D_C.texture);
            if (game.componentManager.hasComponent<InstanceBuffer>(owner))
            {
                auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(owner);
                setFloat("uvOffset", instanceBuffer_C.uvOffset, shader_C.get_shader_program());
                glDrawElementsInstanced(GL_TRIANGLES, model2D_C.numberOfIndices, GL_UNSIGNED_INT, nullptr,
                                        instanceBuffer_C.instances.size());
            }
            else
            {
                auto model = game.componentManager.getComponent<engine::sceneGraph::Transform>(owner).modelMatrix;
                auto mvp = engine.calculateMvpMatrix(model);
                setMatrix("mvp", mvp, shader_C.get_shader_program());
                glDrawElements(GL_TRIANGLES, model2D_C.numberOfIndices, GL_UNSIGNED_INT, nullptr);
            }
            glBindVertexArray(0);
        }
    }

    void RenderSystem::update(engine::Game& game) {
        auto& instanceBufferContainer = game.componentManager.getContainer<InstanceBuffer>();
        for (auto& [owner, _] : instanceBufferContainer)
        {
            auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(owner);
            glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_C.VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, instanceBuffer_C.instances.size() * sizeof(glm::mat4),
                            instanceBuffer_C.instances.data());
        }
    }

    void RenderSystem::initShaders(engine::Game& game) {
        instanceVertexShader = loadAndCompileShader(GL_VERTEX_SHADER, "shaders/batch/vertexShader.vert");
        instanceFragmentShader = loadAndCompileShader(GL_FRAGMENT_SHADER, "shaders/batch/fragmentShader.frag");
        instanceProgram = glCreateProgram();
        glAttachShader(instanceProgram, instanceVertexShader);
        glAttachShader(instanceProgram, instanceFragmentShader);
        glLinkProgram(instanceProgram);
        glDetachShader(instanceProgram, instanceVertexShader);
        glDetachShader(instanceProgram, instanceFragmentShader);

        singleVertexShader = loadAndCompileShader(GL_VERTEX_SHADER, "shaders/single/vertexShader.vert");
        singleFragmentShader = loadAndCompileShader(GL_FRAGMENT_SHADER, "shaders/single/fragmentShader.frag");
        singleProgram = glCreateProgram();
        glAttachShader(singleProgram, singleVertexShader);
        glAttachShader(singleProgram, singleFragmentShader);
        glLinkProgram(singleProgram);
        glDetachShader(singleProgram, singleVertexShader);
        glDetachShader(singleProgram, singleFragmentShader);

        auto& shaderContainer = game.componentManager.getContainer<Shader>();
        for (auto& [owner, _] : shaderContainer)
        {
            auto& shader_C = game.componentManager.getComponent<Shader>(owner);
            if (game.componentManager.hasComponent<InstanceBuffer>(owner))
            {
                shader_C.set_shader_program(instanceProgram);
            }
            else
            {
                shader_C.set_shader_program(singleProgram);
            }
        }
    }

    void RenderSystem::initBuffers(engine::Game& game) {
        auto& model2DContainer = game.componentManager.getContainer<Model2D>();
        for (auto& [owner, _] : model2DContainer)
        {
            auto& model2D_C = game.componentManager.getComponent<Model2D>(owner);
            model2D_C.VBO = createBuffer(GL_ARRAY_BUFFER, model2D_C.vertices);
            model2D_C.EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, model2D_C.indices);
            glGenVertexArrays(1, &model2D_C.VAO);
            glBindVertexArray(model2D_C.VAO);

            // VBO
            glBindBuffer(GL_ARRAY_BUFFER, model2D_C.VBO);
            // Position (location = 0)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // Color (location = 1)
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // TexCoords (location = 2)
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model2D_C.EBO);
            if (game.componentManager.hasComponent<InstanceBuffer>(owner))
            {
                auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(owner);
                glGenBuffers(1, &instanceBuffer_C.VBO);
                glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_C.VBO);
                glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
                glBindVertexArray(model2D_C.VAO);
                for (int i = 0; i < 4; ++i)
                {
                    glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                          (void*)(i * sizeof(glm::vec4)));
                    glEnableVertexAttribArray(3 + i);
                    glVertexAttribDivisor(3 + i, 1);
                }
            }
            glBindVertexArray(0);
        }
    }

    void RenderSystem::deleteBuffers(engine::Game& game) {
        auto& model2DContainer = game.componentManager.getContainer<Model2D>();
        for (auto& [owner, _] : model2DContainer)
        {
            auto& model2D_C = game.componentManager.getComponent<Model2D>(owner);
            glDeleteBuffers(1, &model2D_C.VBO);
            glDeleteBuffers(1, &model2D_C.EBO);
            if (game.componentManager.hasComponent<InstanceBuffer>(owner))
            {
                auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(owner);
                glDeleteBuffers(1, &instanceBuffer_C.VBO);
            }
            glDeleteVertexArrays(1, &model2D_C.VAO);
        }
    }

    void RenderSystem::deleteShader(engine::Game& game) {
        glDeleteShader(instanceVertexShader);
        glDeleteShader(instanceFragmentShader);
        glDeleteShader(singleVertexShader);
        glDeleteShader(singleFragmentShader);
    }

    void RenderSystem::setMatrix(const std::string& uniformName, glm::mat4 matrix, GLuint shaderProgram) {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void RenderSystem::setVector(const std::string& uniformName, glm::vec4 vector, GLuint shaderProgram) {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    void RenderSystem::setFloat(const std::string& uniformName, float value, GLuint shaderProgram) {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform1f(uniformLocation, value);
    }

    unsigned int RenderSystem::loadAndCompileShader(GLuint shaderType, const fs::path& shaderPath) {
        auto shaderID = glCreateShader(shaderType);
        auto shaderSource = readText(shaderPath);
        auto source = shaderSource.c_str();
        glShaderSource(shaderID, 1, &source, nullptr);
        glCompileShader(shaderID);


        glStatusData compilationStatus{};
        compilationStatus.shaderName = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus.success);
        if (compilationStatus.success == GL_FALSE)
        {
            glGetShaderInfoLog(shaderID, GL_INFO_LOG_LENGTH, nullptr, compilationStatus.infoLog);
            throw std::runtime_error(
                "ERROR: " + std::string(compilationStatus.shaderName) + " shader compilation failed.\n" +
                std::string(compilationStatus.infoLog));
        }
        return shaderID;
    }
} // gl3
