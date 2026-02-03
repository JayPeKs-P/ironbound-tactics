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

    /// @brief Stores all information regarding the transformation (e.g. position, rotation) of an entity.
    ///
    /// Transforms are part of the 'Scene Graph'. They are nodes in tree structure. Transforms may have many children
    /// but only one a single parent. If the parent transform is changed in any way, the effect is applied to all its
    /// child nodes. Transform is a @ref Component, therefore the constructor is private and can only be called by
    /// ComponentManager.
    class Transform final : public Component {
        friend ComponentManager;
        friend Entity;

    public:
        std::set<Transform *> getChildTransforms();
        Transform *getParent();

        /// @brief Sets a pointer to the parent Transform and calls addChild(this) on instance of parent.
        /// @param parentTransform Pointer to the instance of transform this instance should have as parent.
        void setParent(Transform *parentTransform);

        /// @brief Marks all child transforms as deleted.
        void invalidate();

        ~Transform() override;
        Transform(const Transform&) = delete;
        Transform& operator = (const Transform&) = delete;
        Transform& operator = (Transform&&) = delete;

        /// @brief Move constructor, used by ComponentManager in ComponentManager::addComponent() to move a Component to
        /// its corresponding container.
        Transform(Transform &&other) noexcept;
        glm::vec3 localPosition;
        float localZRotation;
        glm::vec3 localScale;
        glm::mat4 modelMatrix;


    protected:

        /// @brief Adds a pointer to a Transform to children.
        /// @param transform Pointer to a Transform that will be registered as a child node.
        void addChild(Transform *transform);

        /// @brief Removes a specified Transform from children.
        /// @param Pointer to a Transform that will be deregistered as a child node.
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