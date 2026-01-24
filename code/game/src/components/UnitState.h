//
// Created by Julian Kalb on 18/07/2025.
//
#pragma once
#include <engine/ecs/Component.h>
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

enum class State {IDLE, MOVING, MOVED, AIMING, FIGHTING, RESETTING, PREPARING};
struct UnitState: Component
{
    friend ComponentManager;
    friend Entity;
    public:
    State state = State::IDLE;
    glm::vec3 oldPos;
    glm::vec3 relativeVec;
    glm::vec3 goal;
    guid_t m_iTarget = gl3::engine::ecs::invalidID;
    std::vector<guid_t> m_TargetedBy;

    float movementSpeed = 0.4;
    float traveledDistance = 0;

private:
    explicit UnitState(guid_t owner):
    Component(owner) {}
};
struct Projectile: Component {
    friend ComponentManager;
    friend Entity;
    guid_t m_iRefComponent;
    int m_iCountActive = 0;
private:
    explicit Projectile(guid_t owner, guid_t iRefEntity = gl3::engine::ecs::invalidID):
    Component(owner),
    m_iRefComponent(iRefEntity){}
};
struct ProjectileState: Component {
    friend ComponentManager;
    friend Entity;
    public:
    State state = State::IDLE;
    glm::vec3 lastPos;
    glm::vec3 startPos;
    glm::vec3 endPos;
    float m_fFlightTime = 0.0f;
    float m_fProjectileSpeed = 3.0f;
    guid_t m_iTarget = gl3::engine::ecs::invalidID;
    float elapsedTime = 0.0f;
    bool m_bUpdated = false;
private:
    explicit ProjectileState(guid_t owner):
    Component(owner) {}
};