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
        guid_t guid;
        if (m_FreeGUIDs.size() < FREE_GUID_BUFFER_SIZE) guid = entityCounter++;
        else [[likely]]
        {
            guid = m_FreeGUIDs.front();
            m_FreeGUIDs.pop();
        }
        Entity entity(guid, componentManager);
        entities[guid] = std::make_unique<Entity>(entity);
        return getEntity(guid);
    }

    Entity &EntityManager::getEntity(const guid_t guid) {
        return *entities.at(guid).get();
    }

    bool EntityManager::IsAlive(guid_t guid) {
        return entities.contains(guid);
    }

    void EntityManager::SetParent(guid_t iChild, guid_t iParent) {
        auto& child_E = getEntity(iChild);

        if (child_E.m_iParentEntity != invalidID) {
            auto& oldParent_E = getEntity(child_E.m_iParentEntity);
            oldParent_E.RemoveChild(iChild);
        }

        child_E.m_iParentEntity = iParent;
        getEntity(iParent).AddChild(iChild);
    }

    void EntityManager::deleteEntity(Entity &entity) {
        if (entity.deleted) return;

        entity.deleted = true;
        entity.deleteAllComponents();
        deleteList.push_back(entity.guid());

        auto children = entity.m_Children;
        for (guid_t iChild: children) {
            deleteEntity(getEntity(iChild));
        }
        if (entity.m_iParentEntity != invalidID) {
            getEntity(entity.m_iParentEntity).RemoveChild(entity.guid());
        }
        entity.m_Children.clear();
        entity.m_iParentEntity = invalidID;
    }

    void EntityManager::purgeEntities() {
        for(auto &guid: deleteList) {
            entities.erase(guid);
            guid_t freeGUID = guid;
            m_FreeGUIDs.push(freeGUID);
        }
        deleteList.clear();
    }
}