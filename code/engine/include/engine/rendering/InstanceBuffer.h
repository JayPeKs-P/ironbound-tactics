//
// Created by Julian Kalb on 17/07/2025.
//
#pragma once

#include "engine/ecs/Component.h"
#include "glm/glm.hpp"
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

/// @ingroup Rendering
/// @brief Component to store the ID of a separate VBO buffer, that is used for instance rendering. Inherits from @ref gl3::engine::ecs::Component
struct InstanceBuffer: Component
{
    friend ComponentManager;
    friend Entity;
    /// Delete copy constructor.
    InstanceBuffer(const InstanceBuffer &other) = delete;
    /// Custom move semantics.
    InstanceBuffer(InstanceBuffer &&other) noexcept
    {
        std::swap(this->VBO, other.VBO);
        this->instances = std::move(other.instances);
    }

    /// @brief ID of the VBO used for instance rendering.
    unsigned int VBO = 0;

    /// @brief Stores the MVP matrices of each instance.
    std::vector <glm::mat4> instances;

    ///@brief Used for animation if texture has animation frames.
    float uvOffset = 0.0f;
private:
    explicit InstanceBuffer(guid_t owner):
    Component(owner) {}
};
