//
// Created by julia on 30/10/2024.
//

#include "Missile.h"

using namespace gl3;

Missile::Missile(Game* game):
Entity(Shader("shaders/shaded/vertexShader.vert",
        "shaders/shaded/fragmentShader.frag"),
        Mesh("gltf/missiles.glb"),
{0.0f, 0.0f, 0.0f},
    0.0f,
    {1, 1, 1},
    {0.8f, 0.8f, 0.8f, 1.0f})
{
}

void Missile::update(Game* game, float deltaTime)
{
    position.y += sin(glm::radians(zRotation + 90)) * speed * deltaTime;
    position.x += cos(glm::radians(zRotation + 90)) * speed * deltaTime;
}
