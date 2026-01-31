//
// Created by Julian Kalb on 18/07/2025.
//
#pragma once
#include <engine/ecs/Component.h>
#include "engine/Events.h"
using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

enum class State {IDLE, MOVING, MOVED,MOVE_TO_SIEGE, USING, AIMING, FIGHTING, RESETTING, PREPARING};
struct UnitState: Component
{
    friend ComponentManager;
    friend Entity;
    public:
    State state = State::IDLE;
    glm::vec3 startPos;
    glm::vec3 oldPos;
    glm::vec3 relativeVec;
    glm::vec3 endPos;

    guid_t m_iParentEntity = gl3::engine::ecs::invalidID;
    guid_t m_iTarget = gl3::engine::ecs::invalidID;
    guid_t m_iTargetParentEntity = gl3::engine::ecs::invalidID;

    std::vector<guid_t> m_TargetedBy;

    float movementSpeed = 0.7;

private:
    explicit UnitState(guid_t owner):
    Component(owner) {}
};
struct Projectile: Component {
    friend ComponentManager;
    friend Entity;
    guid_t m_iRefComponent;
    std::vector<int> m_ActiveProjectileList = {0, 0, 0, 0, 0};
private:
    explicit Projectile(guid_t owner, guid_t iRefEntity = gl3::engine::ecs::invalidID):
    Component(owner),
    m_iRefComponent(iRefEntity){}
};
struct ProjectileState: Component {
    friend ComponentManager;
    friend Entity;
    using event_t = gl3::engine::events::Event<ProjectileState, int>;
    event_t m_ComponentDestroyed;

    ~ProjectileState() override {
        m_ComponentDestroyed.InvokeAndClear(this->m_iDelayTurns);
    }
    ProjectileState(ProjectileState&& other) noexcept:
    m_ComponentDestroyed(std::move(other.m_ComponentDestroyed)),
    state(other.state),
    lastPos(other.lastPos),
    startPos(other.startPos),
    endPos(other.endPos),
    m_fFlightTime(other.m_fFlightTime),
    m_fProjectileSpeed(other.m_fProjectileSpeed),
    m_iTarget(other.m_iTarget),
    m_iTargetParentEntity(other.m_iTargetParentEntity),
    elapsedTime(other.elapsedTime),
    m_bUpdated(other.m_bUpdated),
    m_iDelayTurns(other.m_iDelayTurns)
    {
         owner = other.owner;
    }
    ProjectileState(const ProjectileState&) = delete;
    ProjectileState& operator=(const ProjectileState&) = delete;
    ProjectileState& operator=(ProjectileState&&) = delete;

    public:
    State state = State::IDLE;
    glm::vec3 lastPos;
    glm::vec3 startPos;
    glm::vec3 endPos;
    float m_fFlightTime = 0.0f;
    float m_fProjectileSpeed = 3.0f;

    guid_t m_iTarget = gl3::engine::ecs::invalidID;
    guid_t m_iTargetParentEntity = gl3::engine::ecs::invalidID;

    float elapsedTime = 0.0f;
    bool m_bUpdated = false;
    int m_iDelayTurns;
private:
    explicit ProjectileState(guid_t owner, int iAnimationDelay):
    Component(owner),
    m_iDelayTurns(iAnimationDelay){}
};