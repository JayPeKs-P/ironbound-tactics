//
// Created by Julian Kalb on 17/07/2025.
//

#pragma once
#include <vector>

#include "engine/ecs/Component.h"
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

/// @ingroup Rendering
/// @brief Component that stores the ID of rendering buffers, shape vertices and a texture ID.
struct Model2D: Component
{
    friend ComponentManager;
    friend Entity;
    Model2D(const Model2D &other) = delete;
    Model2D(Model2D &&other) noexcept
    {
        std::swap(this->texture, other.texture);
        std::swap(this->VAO, other.VAO);
        std::swap(this->VBO, other.VBO);
        std::swap(this->EBO, other.EBO);
        std::swap(this->numberOfIndices, other.numberOfIndices);
        std::swap(this->vertices,  other.vertices);
        std::swap(this->indices, other.indices);
        // std::swap(this->texture, other.texture);
    }
    /// @brief Vertices of the mesh.
    ///
    /// Coordinates are 3D so every three entries of the vector starts a new vertex.
    std::vector<float> vertices;

    /// @brief Describes the shape of the mesh.
    ///
    /// Used for EBO. Describes triangles so every three entries is a new triangle.
    std::vector<unsigned int> indices;
    unsigned int texture;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    /// @brief For now this Component is used to describe quads only. Therefore, numberOfIndices is set to 6 here.
    unsigned int numberOfIndices = 6;
private:
    explicit Model2D(guid_t owner,std::vector<float> &vertices, std::vector<unsigned int> &indices, unsigned int texID):
    Component(owner),
    vertices(vertices),
    indices(indices),
    texture(texID){}
};