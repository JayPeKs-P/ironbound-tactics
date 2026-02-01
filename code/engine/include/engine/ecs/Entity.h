//
// Created by Julian Kalb on 20/06/2025.
//

// Entity.h
#pragma once
#include <vector>
#include "ecs.h"
#include "engine/ecs/ComponentManager.h"

namespace gl3::engine::ecs {
    /// @brief Abstract base class for all ECS entities.
    ///
    /// Entity primarily consists of  its ID. It has a reference to @ref gl3::engine::ecs::ComponentManager so it can
    /// call its utility functions. Entity is declared final to prevent inheritance. To remove an entity, it is first
    /// flagged for deletion and then purged later during the update cycle by @ref gl3::engine::ecs::EntityManager.
    class Entity final {
        friend class EntityManager;

    public:

        /// @brief Returns the entities unique ID.
        [[nodiscard]] guid_t guid() const { return id; }

        /// @brief Returns boolean, that represents if the entity is marked for deletion.
        [[nodiscard]] bool isDeleted() const { return deleted; }

        /// @brief Adds a component of type 'C' to an instance of @ref gl3::engine::ecs::ComponentManager.
        ///
        /// Does not add the component directly to entity, but an associated container stored in
        /// @ref gl3::engine::ecs::ComponentManager (also calls @ref gl3::engine::ecs::ComponentManager::addComponent()).
        /// @tparam C Type of the @ref gl3::engine::ecs::Component to add.
        /// @tparam Args Arguments, that are used in the specific component's constructor (except guid_t).
        /// @return Reference to the newly created component.
        template<typename C, typename ... Args>
        C &addComponent(Args ...args) {
            return componentManager.addComponent<C>(id, args...);
        }

        /// @brief Get a reference to a specific component, that is associated to this instance of entity.
        ///
        /// Calls @ref gl3::engine::ecs::ComponentManager::getComponent().
        /// @tparam C Type of the @ref gl3::engine::ecs::Component to get.
        /// @return Reference to the component.
        template<typename C>
        C &getComponent() {
            return componentManager.getComponent<C>(id);
        }

        /// @brief Marks the specified @ref gl3::engine::ecs::Component for deletion.
        ///
        /// Calls @ref gl3::engine::ecs::ComponentManager::removerComponent(). This version is used, when a
        /// component that has already been used before deletion should be deleted.
        /// @tparam C Type of the @ref gl3::engine::ecs::Component to add to the deleteList.
        /// @param component Reference to the exact instance of the component, that should be deleted.
        template<typename C>
        void removeComponent(Component &component) {
            component.deleted = true;
            componentManager.removeComponent<C>(id);
        }

        /// @brief Marks a type of @ref gl3::engine::ecs::Component for deletion.
        ///
        /// Calls @ref gl3::engine::ecs::ComponentManager::removerComponent(). This version is used, when the
        /// component has not been used yet. It gets the reference to the component of the specified type.
        /// @tparam C Type of the @ref gl3::engine::ecs::Component to add to the deleteList.
        template<typename C>
        void removeComponent() {
            auto &component = getComponent<C>();
            removeComponent<C>(component);
        }

        std::vector<guid_t> &GetChildren() {
            return m_Children;
        }

    protected:
        /// @param id The ID this instance of entity will be associated to.
        /// @param componentManager A reference to the @ref gl3::engine::ecs::ComponentManager instance.
        explicit Entity(guid_t id, ComponentManager &componentManager) : id(id), componentManager(componentManager) {}

        /// @brief Removes all @ref gl3::engine::ecs::Components associated to this instance of entity.
        ///
        /// This function calls @ref gl3::engine::ecs::ComponentManager::removeComponents().
        void deleteAllComponents() const {
            componentManager.removeComponents(id);
        }

        void AddChild(guid_t iChild) {
            m_Children.push_back(iChild);
        }

        void RemoveChild(guid_t iChild) {
            std::erase(m_Children, iChild);
        }

        /// @brief The entity's ID. By default, it is set to @ref gl3::engine::ecs::invalidID.
        guid_t id = invalidID;
        bool deleted = false;
        ComponentManager &componentManager;
        std::vector<guid_t> m_Children;
        guid_t m_iParentEntity = invalidID;
    };
}