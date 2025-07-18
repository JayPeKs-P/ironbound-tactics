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
    class SceneGraphPruner : public System {
    public:
        explicit SceneGraphPruner(Game &game) : System(game) {
            game.onBeforeUpdate.addListener([&](Game &game) {
                pruneTransforms(game);
            });
        }

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