//
// Created by Julian Kalb on 20/06/2025.
//
#include "engine/ecs/ComponentManager.h"
#include "engine/Game.h"

namespace gl3::engine::ecs {
    ComponentManager::ComponentManager(Game &engine) {
        engine.onAfterUpdate.addListener([&](engine::Game &) {
           purgeComponents();
        });
    }

    void ComponentManager::removeComponents(guid_t entityID) {
        for(auto &[componentTypeID, componentContainer]: containers) {
            const auto &component = componentContainer.find(entityID);
            const auto componentExistsInContainer = component != componentContainer.end();
            if(componentExistsInContainer) component->second->deleted = true;
            deleteList.emplace_back(componentTypeID, entityID);
        }
    }

    void ComponentManager::purgeComponents() {
        for(const auto &[componentTypeID, ownerID]: deleteList) {
            containers.at(componentTypeID).erase(ownerID);
        }
        deleteList.clear();
    }
}