//
// Created by Julian Kalb on 20/06/2025.
//

#pragma once
#include <map>
#include <memory>
#include <stack>
#include <vector>
#include "engine/ecs/ecs.h"
#include "engine/ecs/Entity.h"
#include "engine/ecs/ComponentManager.h"

namespace gl3::engine {
    class Game;
}

namespace gl3::engine::ecs {
    /// @brief The class that handles the lifetime of @ref gl3::engine::ecs::Entity objects.
    ///
    /// An object of this class is member of @ref @ref gl3::engine::Game. Use it to create new entities.
    /// It adds a unique_ptr of a new entity to a container and maps it to its unique ID of type
    /// @ref gl3::engine::ecs::guid_t.
    class EntityManager {
    public:

        /// @brief Adds the member function purgeEntities() to @ref gl3::engine::Game::onAfterUpdate as listener.
        ///
        /// @param componentManager A reference to the core games instance of @ref gl3::engine::ecs::ComponentManager.
        /// @param game A reference to the instance of the core @ref gl3::engine::Game
        EntityManager(ComponentManager &componentManager, engine::Game &game);

        /// @brief Create a new instance of @ref gl3::engine::ecs::Entity and add it to the container entities.
        ///
        /// Assigns the new entity an ID. This ID is the current value of
        /// @ref gl3::engine::ecs::EntityManager::entityCounter (the value is then increased by one).
        /// @return Reference to the ID of the new entity.
        Entity &createEntity();

        /// @brief Gets a reference to a @ref gl3::engine::ecs::Entity object.
        ///
        /// @param guid ID of the target entity.
        /// @return Reference to the entity object associated to the passed ID.
        [[nodiscard]] Entity &getEntity(guid_t guid);

        /// @brief Function to check, if one or more entities have the specified component.
        ///
        /// This utility function returns true, if all entities, whose IDs are provided, have a Component of type T.
        /// @tparam  T Component type to check.
        /// @tparam Guids List of guid_t values of the entities to check.
        /// @return true if all provided entities have component T, false otherwise.
        template <typename T, typename... Guids>
        bool checkIfEntityHasComponent(Guids... guids)
        {
            return (... && componentManager.hasComponent<T>((guids)));
        }

        /// @brief Add a specific entity to the @ref gl3::engine::ecs::EntityManager::deleteList
        void deleteEntity(Entity &entity);

    private:
        /// @brief Erases all @ref gl3::engine::ecs::Entity in the deleteList from the entities container.
        ///
        /// This funcion is passed to @ref gl3::engine::Game::onAfterUpdate.
        void purgeEntities();

        ComponentManager &componentManager;

        /// @brief Stores a unique_ptr to all entities.
        std::map<guid_t, std::unique_ptr<Entity>> entities;
        std::vector<guid_t> deleteList;

        /// @brief Mirrors the amount of total created entities. Used to evaluate the ID of the next created entityg
        int entityCounter = 0;
        std::stack<guid_t> m_FreeGUIDs {};
    };
}