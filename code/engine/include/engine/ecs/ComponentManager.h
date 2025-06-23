//
// Created by Julian Kalb on 20/06/2025.
//
#pragma once
#include <map>
#include <vector>
#include <memory>
#include <typeinfo>
#include <utility>
#include <functional>
#include "engine/ecs/ecs.h"
#include "engine/ecs/Component.h"

namespace gl3::engine {
    class Game;
}

namespace gl3::engine::ecs {
    class ComponentManager {
        using ComponentContainer = std::map<guid_t, std::unique_ptr<Component>>;
    public:
        explicit ComponentManager(engine::Game &engine);

        template<typename C>
        [[nodiscard]] C &addComponent(guid_t owner) {
            C component(owner);
            containers[typeid(C).hash_code()][owner] = std::make_unique<C>(std::move(component));
            return getComponent<C>(owner);
        }

        template<typename C>
        C &getComponent(guid_t owner) {
            return *reinterpret_cast<C*>(containers.at(typeid(C).hash_code()).at(owner).get());
        }

        template<typename C>
         bool hasComponent(const guid_t &owner) {
            auto &container = containers[typeid(C).hash_code()];
            return container.find(owner) != container.end();
        }

        template<typename C>
        ComponentContainer &getContainer() {
            return containers[typeid(C).hash_code()];
        }

        template<typename C>
        void forEachComponent(std::function<void(C&)> f) {
            for(const auto &[_, component]: containers.at(typeid(C).hash_code())) {
                f(*reinterpret_cast<C*>(component.get()));
            }
        }

        template<typename C>
        void removeComponent(guid_t entityID) {
            deleteList.emplace_back( typeid(C).hash_code(), entityID);
        }

        void removeComponents(guid_t entityID);

    private:
        void purgeComponents();

        using ComponentContainer = std::map<guid_t, std::unique_ptr<Component>>;
        std::map<size_t, ComponentContainer> containers;
        std::vector<std::pair<size_t, guid_t>> deleteList;
    };
}