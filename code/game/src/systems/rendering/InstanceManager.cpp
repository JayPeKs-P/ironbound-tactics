//
// Created by Julian Kalb on 18/07/2025.
//

#include "InstanceManager.h"
#include "engine/sceneGraph/Transform.h"
#include "../../components/InstanceBuffer.h"
#include "../../components/unitTypes/Unit.h"
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
            int createdInstances = 0;
            int maxInstances = 0;
            if (game.componentManager.hasComponent<Unit>(transform.entity()))
            {
                maxInstances = game.componentManager.getComponent<Unit>(transform.entity()).lifetimeMaxAmount;
            }
            if (game.componentManager.hasComponent<InstanceBuffer>(transform.entity()))
            {
                auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(transform.entity());
                auto rootLocation = transform.localPosition;
                auto rootScale = transform.localScale;
                for (int j = 0 ; j < 10 && createdInstances < maxInstances; j++)
                {
                    for (int i = 0 ; i < 5 && createdInstances < maxInstances; i++)
                    {
                        auto instance_E = game.entityManager.createEntity();
                        instance_E.addComponent<Transform>(&transform,
                            glm::vec3(rootLocation.x + j * 0.1f, rootLocation.y - i * 0.05, rootLocation.z),
                            0,
                            glm::vec3(rootScale.x, rootScale.y, rootScale.z));

                        createdInstances++;
                    }
                }
            }
        });
    }

    void InstanceManager::update(engine::Game& game, Transform* root)
    {
        int amount = 0;
        int amountLastFrame = 0;
        if (game.componentManager.hasComponent<Unit>(root->entity()))
        {
            amount = game.componentManager.getComponent<Unit>(root->entity()).totalAmount;
            amountLastFrame = game.componentManager.getComponent<Unit>(root->entity()).totalAmountLastFrame;
            game.componentManager.getComponent<Unit>(root->entity()).totalAmountLastFrame = amount;;
        }
        int totalFrames = 4;
        float frameDuration = 0.1f;
        int currentFrame = static_cast<int>(game.elapsedTime / frameDuration) % totalFrames;
        auto& instanceBuffer_C = game.componentManager.getComponent<InstanceBuffer>(root->entity());
        instanceBuffer_C.instances.clear();
        game.componentManager.forEachComponentRevers<Transform>([&](Transform &transform)
        {
            if (transform.getParent() == root)
            {
                if (amount < amountLastFrame)
                {
                    game.entityManager.deleteEntity(game.entityManager.getEntity(transform.entity()));
                    amountLastFrame--;
                }else if (amountLastFrame < amount)
                {
                }else
                {
                    instanceBuffer_C.instances.push_back(game.calculateMvpMatrix(transform.modelMatrix));
                }
            }
        });
        instanceBuffer_C.uvOffset = currentFrame * (1.0f / totalFrames);
//TODO add push function to trigger when unit dies or is added
    }
} // gl3