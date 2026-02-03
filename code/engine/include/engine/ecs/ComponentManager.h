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

    /// @brief The class that handles the lifetime of @ref Component objects.
    ///
    /// An object of this class is member of @ref Game. Use it to add components to an
    /// @ref Entity. It adds a unique_ptr of a new component to a container, and maps it to their
    /// owners ID. For each type of component a new container is created. Because of that, all components of the same
    /// type live in sequential memory.
    class ComponentManager {
        using ComponentContainer = std::map<guid_t, std::unique_ptr<Component>>;
    public:

        /// @brief Adds the member function purgeComponents() to @ref Game::onAfterUpdate as listener.
        ///
        /// @param engine A reference to the base instance of game.
        explicit ComponentManager(engine::Game &engine);

        /// @brief Create a new component of type 'C' and add it to an entity's associated container.
        ///
        /// Creates a new component and moves it into the proper container of its type. If the respective container
        /// does not yet exist, it is created.
        ///
        /// @tparam C The type of the component.
        /// @tparam Args Additional Arguments, that are used in 'C's constructor.
        /// @return A reference to the component we just created.
        template<typename C, typename ...Args>
        [[nodiscard]] C &addComponent(guid_t owner, Args ...args) {
            C component(owner, args...);
            containers[typeid(C).hash_code()][owner] = std::make_unique<C>(std::move(component));
            return getComponent<C>(owner);
        }

        /// @brief Get a specific component from an entity.
        ///
        /// This function uses the component type and owner ID to retrieve the component from its container.
        /// It uses the std::map::at function, so a std::out of range exception fires, should the component not exist.
        /// @tparam C The type of the component the function shall return.
        /// @param owner The ID of the entity the component belongs to.
        /// @return A reference to a component of type 'C', that belongs to the entity with the specified ID.
        template<typename C>
        C &getComponent(guid_t owner) {
            return *reinterpret_cast<C*>(containers.at(typeid(C).hash_code()).at(owner).get());
        }

        /// @brief Check if an entity has a component of type 'C'
        ///
        /// @tparam C The type of the component to check.
        /// @param owner Const reference to the ID of the entity that is checked to have the component.
        /// @return True if containers has a container of type 'C', which belongs to 'owner'.
        template<typename C>
         bool hasComponent(const guid_t &owner) {
            auto &container = containers[typeid(C).hash_code()];
            return container.find(owner) != container.end();
        }

        /// @brief Get the reference to the container, where all components of type C are stored in.
        ///
        /// @return Reference to the container, that has typeid(C) as its key.
        template<typename C>
        ComponentContainer &getContainer() {
            return containers[typeid(C).hash_code()];
        }

        /// @brief A callback function that operates on all components of the specified type 'C'.
        ///
        /// A callback function, that iterates over the
        /// @tparam C The type of the component to iterate over.
        /// @param f A function object, executed for each component, gets reference to current iteration.
        template<typename C>
        void forEachComponent(std::function<void(C&)> f) {
            for(const auto &[_, component]: containers.at(typeid(C).hash_code())) {
                f(*reinterpret_cast<C*>(component.get()));
            }
        }

        /// @brief Does the same as @ref gl3::engine::ecs::ComponentManager::forEachComponent(), but backwards.
        template<typename C>
        void forEachComponentRevers(std::function<void(C&)> f)
        {
            auto &container = containers.at(typeid(C).hash_code());
            for (auto key = container.rbegin(); key != container.rend(); key++)
            {
                f(*reinterpret_cast<C*>(key->second.get()));
            }
        }

        /// @brief Adds the specified component to the deleteList. The component will be erased onAfterUpdate
        template<typename C>
        void removeComponent(guid_t entityID) {
            deleteList.emplace_back( typeid(C).hash_code(), entityID);
        }

        /// @brief Removes all components that belong to a particular entity. Used whenever an entity is destroyed.
        void removeComponents(guid_t entityID);

    private:

        /// @brief Erases all @ref gl3::engine::ecs::Component in the deleteList from their respective container.
        ///
        /// This function is passed to @ref gl3::engine::Game::onAfterUpdate.
        void purgeComponents();

        using ComponentContainer = std::map<guid_t, std::unique_ptr<Component>>;
        std::map<size_t, ComponentContainer> containers;
        std::vector<std::pair<size_t, guid_t>> deleteList;
    };
}