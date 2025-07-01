//
// Created by Julian Kalb on 20/06/2025.
//

// Entity.h
#pragma once
#include <vector>
#include "ecs.h"
#include "engine/ecs/ComponentManager.h"

namespace gl3::engine::ecs {
    class Entity final {
        friend class EntityManager;

    public:
        [[nodiscard]] guid_t guid() const { return id; }
        [[nodiscard]] bool isDeleted() const { return deleted; }

        template<typename C, typename ... Args>
        [[nodiscard]] C &addComponent(Args ...args) {
            return componentManager.addComponent<C>(id, args...);
        }

        template<typename C>
        C &getComponent() {
            return componentManager.getComponent<C>(id);
        }

        template<typename C>
        void removeComponent(Component &component) {
            component.deleted = true;
            componentManager.removeComponent<C>(id);
        }

        template<typename C>
        void removeComponent() {
            auto &component = getComponent<C>();
            removeComponent<C>(component);
        }

    private:
        explicit Entity(guid_t id, ComponentManager &componentManager) : id(id), componentManager(componentManager) {}
        void deleteAllComponents() {
            componentManager.removeComponents(id);
        }

        guid_t id = invalidID;
        bool deleted = false;
        ComponentManager &componentManager;
    };
}