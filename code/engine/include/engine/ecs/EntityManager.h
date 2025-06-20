//
// Created by Julian Kalb on 20/06/2025.
//

#pragma once
#include <map>
#include <memory>
#include <vector>
#include "engine/ecs/ecs.h"
#include "engine/ecs/Entity.h"
#include "engine/ecs/ComponentManager.h"

namespace gl3::engine {
    class Game;
}

namespace gl3::engine::ecs {
    class EntityManager {
    public:
        EntityManager(ComponentManager &componentManager, engine::Game &game);

        Entity &createEntity();
        [[nodiscard]] Entity &getEntity(guid_t guid);
        void deleteEntity(Entity &entity);

    private:
        void purgeEntities();

        ComponentManager &componentManager;
        std::map<guid_t, std::unique_ptr<Entity>> entities;
        std::vector<guid_t> deleteList;
        int entityCounter = 0;
    };
}