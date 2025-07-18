//
// Created by Julian Kalb on 18/07/2025.
//

#include "InstanceManager.h"
#include "engine/sceneGraph/Transform.h"
#include "../../components/InstanceBuffer.h"
#include "../../components/UnitState.h"
#include "../../components/Shader.h"
using gl3::engine::sceneGraph::Transform;
namespace gl3 {
    InstanceManager::InstanceManager(engine::Game& game):
    System(game)
    {
        game.onAfterStartup.addListener([&](engine::Game &game)
        {
            init(game);
        });
    }

    void InstanceManager::init(engine::Game &game)
    {
    game.componentManager.forEachComponent<Transform>([&](Transform &transform)
    {
        if (game.componentManager.hasComponent<InstanceBuffer>(transform.entity()))
        {
            auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(transform.entity());
            auto rootLocation = transform.localPosition;
            auto rootScale = transform.localScale;
            for (int i = 0 ; i < 5 ; i++)
            {
                for (int j = 0 ; j < 10; j++)
                {
                    auto instance_E = game.entityManager.createEntity();
                    instance_E.addComponent<Transform>(&transform,
                        glm::vec3(rootLocation.x + j * 0.1f, rootLocation.y + i * 0.05, rootLocation.z),
                        0,
                        glm::vec3(rootScale.x, rootScale.y, rootScale.z));
                    instance_E.addComponent<UnitState>();
                }
            }
        }
    });
    }

    void InstanceManager::update(engine::Game& game, Transform* root)
    {
        int totalFrames = 4;
        float frameDuration = 0.1f;
        int currentFrame = static_cast<int>(game.elapsedTime / frameDuration) % totalFrames;
        game.componentManager.forEachComponent<Transform>([&](Transform &transform)
        {
            if (transform.getParent() == root)
            {
                auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(root->entity());
                instanceBuffer_C.instances.push_back(game.calculateMvpMatrix(transform.modelMatrix));
                instanceBuffer_C.uvOffset = currentFrame * (1.0f / totalFrames);
            }
        });

    }
} // gl3