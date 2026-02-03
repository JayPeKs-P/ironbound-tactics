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
    // /// Delete copy constructor.
    // InstanceBuffer(const InstanceBuffer &other) = delete;
    // /// Custom move semantics.
    // InstanceBuffer(InstanceBuffer &&other) noexcept
    // {
    //     std::swap(this->VBO, other.VBO);
    //     this->instances = std::move(other.instances);
    // }
    /// @brief Stores the MVP matrices of each instance.
    std::vector <glm::mat4> instances;
    std::vector <glm::vec2> instanceUVs;

    /// @brief ID of the VBO used for instance rendering.
    unsigned int VBO = 0;
    unsigned int uvVBO = 0;


    ///@brief Used for animation if texture has animation frames.
    // float uvOffset = 0.0f;
private:
    explicit InstanceBuffer(guid_t owner):
    Component(owner) {}
};
enum UnitDirection : uint8_t{DOWN, LEFT, RIGHT, UP};
enum UnitAffiliation : uint8_t{PLAYER = RIGHT, ENEMY = LEFT};
struct UvOffset: Component {
    friend ComponentManager;
    friend Entity;

    float u = 0.0f;
    float v = 0.0f;
    int m_iHorizontalSize = 0;
    int m_iVerticalSize = 0;

    const float originalV;
    const float originalU;

private:
    explicit UvOffset(guid_t owner, float v, float u, int hSize, int vSize):
    Component(owner),
    originalV(v),
    originalU(u),
    v(v),
    u(u),
    m_iHorizontalSize(hSize),
    m_iVerticalSize(vSize){}
};
struct AnimationSpeed: Component {
    friend ComponentManager;
    friend Entity;

    float m_fFrameTime;
private:
    explicit AnimationSpeed(guid_t owner, float fFrameTime):
    Component(owner),
    m_fFrameTime(fFrameTime){}
};

struct Visibility: Component {
    friend ComponentManager;
    friend Entity;
    bool m_bVisible;
private:
    explicit Visibility(guid_t owner, bool bVisible):
    Component(owner),
    m_bVisible(bVisible){}
};
struct InstanceAmount: Component {
    friend ComponentManager;
    friend Entity;

    uint32_t value;
    uint32_t last;

private:
    explicit InstanceAmount(guid_t owner, uint32_t initialAmount = 0):
    Component(owner),
    value(initialAmount),
    last(initialAmount){}
};
