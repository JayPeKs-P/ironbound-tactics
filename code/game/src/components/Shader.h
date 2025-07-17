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

struct Shader: Component
{
    friend ComponentManager;
    friend Entity;

    [[nodiscard]] unsigned int get_shader_program() const
    {
        return shaderProgram;
    }

    void set_shader_program(unsigned int shader_program)
    {
        shaderProgram = shader_program;
    }

    [[nodiscard]] unsigned int get_vertex_shader() const
    {
        return vertexShader;
    }

    void set_vertex_shader(unsigned int vertex_shader)
    {
        vertexShader = vertex_shader;
    }

    [[nodiscard]] unsigned int get_fragment_shader() const
    {
        return fragmentShader;
    }

    void set_fragment_shader(unsigned int fragment_shader)
    {
        fragmentShader = fragment_shader;
    }

    Shader(const Shader &shader) = delete;
    Shader(Shader &&other) noexcept
    {
        std::swap(this->shaderProgram, other.shaderProgram);
        std::swap(this->vertexShader, other.vertexShader);
        std::swap(this->fragmentShader, other.fragmentShader);
    }
private:
    explicit Shader(guid_t owner):
    Component(owner){}
    unsigned int shaderProgram;
    unsigned int vertexShader;
    unsigned int fragmentShader;
};
