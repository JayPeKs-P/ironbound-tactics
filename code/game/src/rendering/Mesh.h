//
// Created by Julian Kalb on 23/10/2024.
//
#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace gl3 {
    struct InstanceData
    {
        glm::mat4 modelMatrix;
        // float uvOffset;
    };
    class Mesh {
    public:
        explicit Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int texID);
        void draw(int instanceCount) const;
        void update(const std::vector<InstanceData> &instances);
        ~Mesh();

        // Delete copy constructor
        Mesh(const Mesh &other) = delete;

        // Explicit move constructor
        Mesh(Mesh &&other) noexcept {
            std::swap(this->VAO, other.VAO);
            std::swap(this->VBO, other.VBO);
            std::swap(this->EBO, other.EBO);
            std::swap(this->instanceVBO, other.instanceVBO);
            std::swap(this->numberOfIndices, other.numberOfIndices);
        }

    private:
        unsigned int instanceVBO = 0;
        unsigned int texture;
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int numberOfIndices = 0;
    };
}
