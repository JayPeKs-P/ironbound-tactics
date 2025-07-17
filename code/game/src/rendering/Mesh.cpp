//
// Created by Julian Kalb on 23/10/2024.
//

#include "Mesh.h"

#include <glad/glad.h>
#include "engine/Texture.h"

using namespace gl3;

template<typename T>
GLuint createBuffer(GLuint bufferType, const std::vector<T> &bufferData) {
    unsigned int buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(bufferType, buffer);
    glBufferData(bufferType, bufferData.size() * sizeof(T), bufferData.data(), GL_DYNAMIC_DRAW);
    return buffer;
}

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int texID):
         numberOfIndices(indices.size()),
         VBO(createBuffer(GL_ARRAY_BUFFER, vertices)),
         EBO(createBuffer(GL_ELEMENT_ARRAY_BUFFER, indices)),
         texture(texID)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO: Vertexdaten
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Instance VBO vorbereiten
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 60 * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
    // modelMatrix (location = 3,4,5,6)
    glBindVertexArray(VAO);
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);
    }
    // uvOffset (location = 7)
    // glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::mat4)));
    // glEnableVertexAttribArray(7);
    // glVertexAttribDivisor(7, 1);
    //
    // Fertig
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw(int instanceCount) const
{
    glBindVertexArray(VAO);
    // //test for batching
    // //VBO
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //
    // for (int i = 0; i < 4; i++)
    // {
    //     glEnableVertexAttribArray(3 + i);
    //     glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(i * sizeof(glm::mat4)));
    //     glVertexAttribDivisor(3 + i, 1);
    // }
    // glEnableVertexAttribArray(7);
    // glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(7 * sizeof(glm::mat4)));
    // glVertexAttribDivisor(7, 1);
    // // position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // // texture attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
    // //EBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElementsInstanced(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, nullptr, instanceCount);
    glBindVertexArray(0);
}

void Mesh::update(const std::vector<InstanceData>& instances)
{
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData),instances.data());
}
