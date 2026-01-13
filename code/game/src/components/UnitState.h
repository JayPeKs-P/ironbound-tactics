//
// Created by Julian Kalb on 18/07/2025.
//
#pragma once
#include <engine/ecs/Component.h>
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

enum class State {IDLE, MOVING, MOVED, FIGHTING, RESETTING, PREPARING};
struct UnitState: Component
{
    friend ComponentManager;
    friend Entity;
    public:
    State state = State::IDLE;
    glm::vec3 oldPos;
    glm::vec3 relativeVec;
    glm::vec3 goal;
    // gl3::engine::sceneGraph::Transform* m_pTarget = nullptr;
    // std::vector<UnitState*> m_TargetedBy {};
    guid_t m_iTarget = gl3::engine::ecs::invalidID;
    std::vector<guid_t> m_TargetedBy;

    float movementSpeed = 0.4;
    float traveledDistance = 0;

    // ~UnitState() override {
    //     if (!m_TargetedBy.empty())
    //     {
    //         for (auto& instance : m_TargetedBy)
    //         {
    //             instance->m_pTarget = nullptr;
    //         }
    //     }
    // }

private:
    explicit UnitState(guid_t owner):
    Component(owner) {}
};