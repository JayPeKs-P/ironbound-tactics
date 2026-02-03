//
// Created by Julian Kalb on 17/07/2025.
//
#pragma once
#include <vector>
#include <engine/ecs/Component.h>
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;
/// @ingroup Rendering
/// @brief Component for shader data.
/// Stores the ID of a shader program. Inherits from @ref gl3::engine::ecs::Component
struct Shader: Component
{
    friend ComponentManager;
    friend Entity;

    /// Delete copy constructor.
    Shader(const Shader &shader) = delete;

    /// Custom move semantics.
    Shader(Shader &&other) noexcept
    {
        std::swap(this->shaderProgram, other.shaderProgram);
    }

    [[nodiscard]] unsigned int get_shader_program() const
    {
        return shaderProgram;
    }

    void set_shader_program(unsigned int shader_program)
    {
        shaderProgram = shader_program;
    }

private:
    explicit Shader(guid_t owner):
    Component(owner){}
    unsigned int shaderProgram;
};
