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

// struct InstanceData
// {
//     glm::mat4 model;
// };
struct InstanceBuffer: Component
{
    friend ComponentManager;
    friend Entity;
    InstanceBuffer(const InstanceBuffer &other) = delete;
    InstanceBuffer(InstanceBuffer &&other) noexcept
    {
        std::swap(this->VBO, other.VBO);
        this->instances = std::move(other.instances);
    }
    unsigned int VBO = 0;
    std::vector <glm::mat4> instances;
    float uvOffset = 0.0f;
private:
    explicit InstanceBuffer(guid_t owner):
    Component(owner) {}
};
