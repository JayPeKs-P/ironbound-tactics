//
// Created by Julian Kalb on 18/07/2025.
//

#pragma once
#include <set>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "engine/ecs/ecs.h"
#include "engine/ecs/Component.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::Entity;
using gl3::engine::ecs::guid_t;

namespace gl3::engine::sceneGraph {
    class Transform final : public Component {
        friend ComponentManager;
        friend Entity;

    public:
        std::set<Transform *> getChildTransforms();
        Transform *getParent();
        void setParent(Transform *parentTransform);
        void invalidate();

        ~Transform() override;
        Transform(const Transform&) = delete;
        Transform& operator = (const Transform&) = delete;
        Transform& operator = (Transform&&) = delete;
        Transform(Transform &&other) noexcept;
        glm::vec3 localPosition;
        float localZRotation;
        glm::vec3 localScale;
        glm::mat4 modelMatrix;


    protected:
        void addChild(Transform *transform);
        void removeChild(Transform *transform);

    private:
        explicit Transform(guid_t owner,
                           Transform *parentTransform = nullptr,
                           glm::vec3 position = {0.0f, 0.0f, 0.0f},
                           float zRotation = 0,
                           glm::vec3 scale = {1.0f, 1.0f, 1.0f});
    Transform *parent = nullptr;
    std::set<Transform *> children;
    };
}