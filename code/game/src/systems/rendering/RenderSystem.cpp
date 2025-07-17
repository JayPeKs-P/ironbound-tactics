//
// Created by Julian Kalb on 17/07/2025.
//

#include "RenderSystem.h"
#include <fstream>
#include <sstream>
#include "../../Assets.h"

#include "../../components/Model2D.h"
#include "../../components/Shader.h"

namespace gl3 {
    struct glStatusData
    {
        int success;
        const char *shaderName;
        char infoLog[GL_INFO_LOG_LENGTH];
    };

    std::string readText(const fs::path &filePath) {
        std::ifstream sourceFile(resolveAssetPath(filePath));
        std::stringstream buffer;
        buffer << sourceFile.rdbuf();
        return buffer.str();
    }
    template<typename T>
    GLuint createBuffer(GLuint bufferType, const std::vector<T> &bufferData) {
        unsigned int buffer = 0;
        glGenBuffers(1, &buffer);
        glBindBuffer(bufferType, buffer);
        glBufferData(bufferType, bufferData.size() * sizeof(T), bufferData.data(), GL_DYNAMIC_DRAW);
        return buffer;
    }

    RenderSystem::RenderSystem(engine::Game &game):
    System(game)
    {
        engine.onAfterStartup.addListener([&](engine::Game& game)
        {
            initShaders(game);
            initBuffers(game);
        });
    }

    RenderSystem::~RenderSystem()
    {
        deleteBuffers();
        deleteShader();
    }

    void RenderSystem::draw(engine::Game &game)
    {
        auto& model2DContainer = game.componentManager.getContainer<Model2D>();
        for (auto &[owner, _]: model2DContainer)
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
                glDrawElementsInstanced(GL_TRIANGLES, model2D_C.numberOfIndices, GL_UNSIGNED_INT, nullptr, instanceBuffer_C.instances.size());
            }else
            {
            glDrawElements(GL_TRIANGLES, model2D_C.numberOfIndices, GL_UNSIGNED_INT, nullptr);
            }
            glBindVertexArray(0);
        }
    }

    void RenderSystem::update(InstanceBuffer instanceBuffer_C)
    {
        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_C.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, instanceBuffer_C.instances.size() * sizeof(InstanceData),instanceBuffer_C.instances.data());
    }

    void RenderSystem::initShaders(engine::Game& game)
    {
        auto& shaderContainer = game.componentManager.getContainer<Shader>();
        for (auto &[owner, _]: shaderContainer)
        {
            auto& shader_C = game.componentManager.getComponent<Shader>(owner);
            if (game.componentManager.hasComponent<InstanceBuffer>(owner))
            {
                auto shaderData = loadAndCompileShader(GL_VERTEX_SHADER, "shaders/vertexShader.vert");
                shader_C.set_vertex_shader(shaderData);
                shaderData = loadAndCompileShader(GL_FRAGMENT_SHADER,"shaders/fragmentShader.frag");
                shader_C.set_fragment_shader(shaderData);
            }else
            {
                auto shaderData = loadAndCompileShader(GL_VERTEX_SHADER, "shaders/shaded/vertexShader.vert");
                shader_C.set_vertex_shader(shaderData);
                shaderData = loadAndCompileShader(GL_FRAGMENT_SHADER,"shaders/shaded/fragmentShader.frag");
                shader_C.set_fragment_shader(shaderData);
            }
            shader_C.set_shader_program(glCreateProgram());
            auto vertexShader = shader_C.get_vertex_shader();
            auto fragmentShader = shader_C.get_fragment_shader();
            auto program = shader_C.get_shader_program();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            glDetachShader(program, vertexShader);
            glDetachShader(program, fragmentShader);
        }
    }

    void RenderSystem::initBuffers(engine::Game &game)
    {
        auto& model2DContainer = game.componentManager.getContainer<Model2D>();
        for (auto &[owner, _]: model2DContainer)
        {
            auto& model2D_C = game.componentManager.getComponent<Model2D>(owner);
            model2D_C.VBO = createBuffer(GL_ARRAY_BUFFER, model2D_C.vertices);
            model2D_C.EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, model2D_C.indices);
            glGenVertexArrays(1, &model2D_C.VAO);
            glBindVertexArray(model2D_C.VAO);

            // VBO: Vertexdaten
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

            // EBO: Indexdaten
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model2D_C.EBO);
            if (game.componentManager.hasComponent<InstanceBuffer>(owner))
            {
                auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(owner);
                // Instance VBO vorbereiten
                glGenBuffers(1, &instanceBuffer_C.VBO);
                glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_C.VBO);
                glBufferData(GL_ARRAY_BUFFER, 60 * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
                glBindVertexArray(model2D_C.VAO);
                for (int i = 0; i < 4; ++i)
                {
                    glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(i * sizeof(glm::vec4)));
                    glEnableVertexAttribArray(3 + i);
                    glVertexAttribDivisor(3 + i, 1);
                }
            }
            glBindVertexArray(0);
        }
    }

    void RenderSystem::deleteBuffers()
    {
        auto& model2DContainer = engine.componentManager.getContainer<Model2D>();
        for (auto &[owner, _]: model2DContainer)
        {
            auto& model2D_C = engine.componentManager.getComponent<Model2D>(owner);
            glDeleteBuffers(1, &model2D_C.VBO);
            glDeleteBuffers(1, &model2D_C.EBO);
            if (engine.componentManager.hasComponent<InstanceBuffer>(owner))
            {
                auto& instanceBuffer_C = engine.componentManager.getComponent<InstanceBuffer>(owner);
                glDeleteBuffers(1, &instanceBuffer_C.VBO);
            }
            glDeleteVertexArrays(1, &model2D_C.VAO);
        }
    }

    void RenderSystem::deleteShader()
    {
        auto& shaderContainer = engine.componentManager.getContainer<Shader>();
        for (auto &[owner, _]: shaderContainer)
        {
            auto& shader_C = engine.componentManager.getComponent<Shader>(owner);
            glDeleteShader(shader_C.get_vertex_shader());
            glDeleteShader(shader_C.get_fragment_shader());
        }
    }

    void RenderSystem::setMatrix(const std::string& uniformName, glm::mat4 matrix, GLuint shaderProgram)
    {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void RenderSystem::setVector(const std::string& uniformName, glm::vec4 vector, GLuint shaderProgram)
    {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    void RenderSystem::setFloat(const std::string& uniformName, float value, GLuint shaderProgram)
    {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform1f(uniformLocation, value);
    }

    unsigned int RenderSystem::loadAndCompileShader(GLuint shaderType, const fs::path& shaderPath)
    {
        auto shaderID = glCreateShader(shaderType);
        auto shaderSource = readText(shaderPath);
        auto source = shaderSource.c_str();
        glShaderSource(shaderID, 1, &source, nullptr);
        glCompileShader(shaderID);


        glStatusData compilationStatus{};
        compilationStatus.shaderName = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus.success);
        if(compilationStatus.success == GL_FALSE)
        {
            glGetShaderInfoLog(shaderID, GL_INFO_LOG_LENGTH, nullptr, compilationStatus.infoLog);
            throw std::runtime_error("ERROR: " + std::string(compilationStatus.shaderName) + " shader compilation failed.\n" +
            std::string(compilationStatus.infoLog));
        }
        return shaderID;
    }
} // gl3