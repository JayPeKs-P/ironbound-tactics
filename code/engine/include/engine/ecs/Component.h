//
// Created by Julian Kalb on 20/06/2025.
//

// Component.h
#pragma once
#include "ecs.h"

namespace gl3::engine::ecs {
    /// @brief Abstract base class for all ECS components.
    ///
    /// Every Component has an owner. It also stores the guid_t ID of the entity they belong to. The lifetime of
    /// all components should be managed by an instance of ComponentManager. Component has two friend classes: Entity
    /// and ComponentManager.
    class Component {
        friend class ComponentManager;
        friend class Entity;

    public:
        virtual ~Component() = default;

        /// @brief Get the ID of the entity the component belongs to.
        [[nodiscard]] guid_t entity() const { return owner; }

        /// @brief Check if Component is scheduled to be deleted during the next call of
        /// @ref gl3::engine::ecs::ComponentManager::removeComponents(). This is necessary since systems might delete
        /// components that other systems still want to access. Therefore, components are deleted at the end of the
        /// update cycle.
        [[nodiscard]] bool isDeleted() const { return deleted; }

    protected:
        /// @brief Only called in child classes initializer list.
        ///
        /// @param owner The ID of the entity this component belongs to.
        explicit Component(guid_t owner = invalidID)
                : owner(owner) {};

        guid_t owner;
        bool deleted = false;
    };
}