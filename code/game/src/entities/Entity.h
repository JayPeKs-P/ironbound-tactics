//
// Created by Julian Kalb on 24/10/2024.
//

#pragma once
#include "../rendering/Shader.h"
#include "../rendering/Mesh.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

namespace gl3 {
class Game;
class Entity {
public:
    void setPosition(const glm::vec3& position)
    {
        this->position = position;
    }

    void setScale(const glm::vec3& scale)
    {
        this->scale = scale;
    }

    void setZRotation(float z_rotation)
    {
        zRotation = z_rotation;
    }

    [[nodiscard]] glm::vec3 getPosition() const
    {
        return position;
    }

    [[nodiscard]] glm::vec3 getScale() const
    {
        return scale;
    }

    [[nodiscard]] float getZRotation() const
    {
        return zRotation;
    }

    virtual void draw(Game *game);
    virtual void update(Game* game, float deltaTime){}

    Entity(Shader shader,
           Mesh mesh,
           glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           float zRotation = 0.0f,
           glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    virtual ~Entity() = default;



private:
    glm::mat4 Entity::calculateModelToWorldNormal() const;
    Shader shader;
    Mesh mesh;

protected:
    glm::vec3 position;
    glm::vec3 scale;
    float zRotation;
    glm::vec4 color;
    int lastShot = 0;


};

} // gl3