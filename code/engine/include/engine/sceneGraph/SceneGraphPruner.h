//
// Created by Julian Kalb on 18/07/2025.
//
#pragma once
#include "engine/Game.h"
#include "engine/ecs/System.h"
#include "Transform.h"

using gl3::engine::ecs::System;
using gl3::engine::Game;
using gl3::engine::sceneGraph::Transform;

namespace gl3::engine::sceneGraph {

    /// @brief Handles deletion of Entity instances that have a Transform Component which is marked as deleted.
    class SceneGraphPruner : public System {
    public:

        /// Adds pruneTransforms() to @ref engine::Game::onBeforeUpdate as listener.
        explicit SceneGraphPruner(Game &game) : System(game) {
            game.onBeforeUpdate.addListener([&](Game &game) {
                pruneTransforms(game);
            });
        }

        /// @brief Marks all Entity instances with a Transform Component which is marked as deleted also as deleted.
        ///
        /// Calls ComponentManager::forEachComponent() and ComponentManager::getEntity().
        /// @param game Reference to the core instance of @ref engine::Game
        void pruneTransforms(Game &game) {
            game.componentManager.forEachComponent<Transform>([&](Transform &transform) {
                if(transform.isDeleted()) {
                    auto &entity = game.entityManager.getEntity(transform.entity());
                    game.entityManager.deleteEntity(entity);
                }
            });
        }
    };
}