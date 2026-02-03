//
// Created by Julian Kalb on 18/07/2025.
//
#include <glm/gtc/matrix_transform.hpp>
#include "engine/sceneGraph/Transform.h"

namespace gl3::engine::sceneGraph {
    Transform::Transform(guid_t owner, Transform *parentTransform, glm::vec3 position, float zRotation, glm::vec3 scale)
            : Component(owner),
              localPosition(position),
              localZRotation(zRotation),
              localScale(scale),
              modelMatrix(glm::identity<glm::mat4>()) {
        setParent(parentTransform);
    }

    Transform::Transform(Transform &&other) noexcept:
    localPosition(other.localPosition),
    localZRotation(other.localZRotation),
    localScale(other.localScale),
    modelMatrix(other.modelMatrix),
    children(std::move(other.children)) {
        owner = other.owner;
        setParent(other.parent);
    }

    Transform *Transform::getParent() {
        return parent;
    }

    void Transform::setParent(Transform *parentTransform) {
        parent = parentTransform;
        if(parent != nullptr) {
            parent->addChild(this);
        }
    }

    void Transform::addChild(Transform *transform) {
        children.insert(transform);
    }

    void Transform::removeChild(Transform *transform) {
        children.erase(transform);
    }

    std::set<Transform *> Transform::getChildTransforms() {
        return children;
    }

    void Transform::invalidate() {
        deleted = true;
        parent = nullptr;
        for(auto child: children) {
            child->invalidate();
        }
        children.clear();
    }

    Transform::~Transform() {
        if(parent != nullptr) {
            parent->removeChild(this);
        }
        invalidate();
    }
}