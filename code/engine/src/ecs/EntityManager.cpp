//
// Created by Julian Kalb on 20/06/2025.
//
#include "engine/ecs/EntityManager.h"
#include "engine/Game.h"


namespace gl3::engine::ecs {
    EntityManager::EntityManager(ComponentManager &componentManager, engine::Game &game)
            : componentManager(componentManager) {
        game.onAfterUpdate.addListener([&](engine::Game &) {
            purgeEntities();
        });
    }

    Entity &EntityManager::createEntity() {
        auto guid = entityCounter++;
        Entity entity(guid, componentManager);
        entities[guid] = std::make_unique<Entity>(entity);
        return getEntity(guid);
    }

    Entity &EntityManager::getEntity(const guid_t guid) {
        return *entities.at(guid).get();
    }

    void EntityManager::deleteEntity(Entity &entity) {
        entity.deleted = true;
        entity.deleteAllComponents();
        deleteList.push_back(entity.guid());
    }

    void EntityManager::purgeEntities() {
        for(auto &guid: deleteList) {
            entities.erase(guid);
        }
        deleteList.clear();
    }
}