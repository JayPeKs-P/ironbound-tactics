//
// Created by Julian Kalb on 17/07/2025.
//

#include "RenderSystem.h"

#include "../../components/InstanceBuffer.h"
#include "../../components/Model2D.h"

namespace gl3 {
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
    }

    void RenderSystem::draw(engine::Game &game)
    {
        auto& model2DContainer = game.componentManager.getContainer<Model2D>();
        for (auto &[owner, _]: model2DContainer)
        {
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
            glBindVertexArray(0);
            }
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
} // gl3