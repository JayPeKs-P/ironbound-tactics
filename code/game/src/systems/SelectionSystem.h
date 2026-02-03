//
// Created by Julian Kalb on 21/07/2025.
//

#pragma once
#include <iostream>

#include "engine/ecs/System.h"

namespace gl3 {

class SelectionSystem: public engine::ecs::System {
    using transform_t = typename engine::sceneGraph::Transform;
    public:
    explicit SelectionSystem(engine::Game &game): System(game) {};

    /// @tparam Component that identifies desired entity
    template<typename T, typename C>
    T* select(engine::Game& game, float pickRadius = 0.5f)
    {
        T* selected = nullptr;
        glm::vec3 mouseWorldPos = getMousePos();
        game.componentManager.forEachComponent<transform_t>([&](transform_t &transform)
        {
            float dist = glm::distance(glm::vec2(mouseWorldPos), glm::vec2(transform.localPosition));
            if (dist >= pickRadius) return;
            if (!engine.componentManager.hasComponent<C>(transform.entity())) return;
            auto iRoot = transform.getParent()->entity();
            if (!engine.componentManager.hasComponent<T>(iRoot)) return;
            selected = &engine.componentManager.getComponent<T>(iRoot);
        });
        return selected;
    }
    // private:
    glm::vec3 getMousePos()
    {
        double mouseX, mouseY;
        glfwGetCursorPos(engine.getWindow(), &mouseX, &mouseY);
        int winWidth, winHeight;
        glfwGetWindowSize(engine.getWindow(), &winWidth, &winHeight);

        float ndcX = (2.0f * mouseX / winWidth - 1.0f);
        float ndcY = 1.0f - (2.0f * mouseY / winHeight);
        glm::vec3 rayStart = glm::vec3(ndcX, ndcY, -1.0f);
        glm::vec3 rayEnd = glm::vec3(ndcX, ndcY, 1.0f);

        glm::vec4 rayStartWorld = inverse(engine.calculateMvpMatrix(glm::mat4(1.0f))) * glm::vec4(rayStart, 1.0f);
        rayStartWorld /= rayStartWorld.w;
        glm::vec4 rayEndWorld = inverse(engine.calculateMvpMatrix(glm::mat4(1.0f))) * glm::vec4(rayEnd, 1.0f);
        rayEndWorld /= rayEndWorld.w;

        glm::vec3 rayDir = glm::normalize(glm::vec3(rayEndWorld - rayStartWorld));
        return getZPlaneIntersection(glm::vec3(rayStartWorld), rayDir);
        // glm::mat4 invVPMatrix = glm::inverse(engine.calculateMvpMatrix(glm::mat4(1.0f)));
        // glm::vec4 mouseWorldPos = invVPMatrix * mouseNDC;
        // glm::vec2 mouse2DPos = glm::vec2(mouseWorldPos) / mouseWorldPos.w;
    }
    glm::vec3 getZPlaneIntersection(glm::vec3 origin, glm::vec3 direction)
    {
        float t = -origin.z / direction.z;
        return origin + t * direction;
    }


};

} // gl3