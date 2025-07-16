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
    glBufferData(bufferType, bufferData.size() * sizeof(T), bufferData.data(), GL_STATIC_DRAW);
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
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}