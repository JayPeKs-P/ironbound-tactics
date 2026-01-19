//
// Created by Julian Kalb on 18/07/2025.
//

#include "InstanceManager.h"

#include <iostream>

#include "engine/rendering/InstanceBuffer.h"
#include "../../components/unitTypes/Unit.h"
#include "../../components/UnitState.h"
#include "../../gui/GuiCombat.h"
#include "../../systems/CombatController.h"
#include "engine/sceneGraph/SceneGraphPruner.h"

using gl3::engine::sceneGraph::Transform;
namespace gl3 {
    InstanceManager::InstanceManager(engine::Game& game):
    System(game)
    {
        game.onUpdate.addListener([&](Game& game)
        {
            update(game);
        });

        GuiCombat::startRound.addListener([&]()
        {
        });
        CombatController::initialize.addListener([&]()
        {
            init(engine);
        });
        CombatController::enemyDead.addListener([&]()
        {
            terminate(engine);
        });
        CombatController::playerDead.addListener([&]()
        {
            terminate(engine);
        });
    }

    void InstanceManager::init(engine::Game &game)
    {
        game.componentManager.forEachComponent<Transform>([&](Transform &transform)
        {
            if (!game.componentManager.hasComponent<InstanceBuffer>(transform.entity())) [[likely]] return;

            int createdInstances = 0;
            int maxInstances = 0;
            if (game.componentManager.hasComponent<Unit>(transform.entity()))
            {
                maxInstances = game.componentManager.getComponent<Unit>(transform.entity()).lifetimeMaxAmount;
                auto rootLocation = transform.localPosition;
                auto rootScale = transform.localScale;
                for (int j = 0 ; j < 10 && createdInstances < maxInstances; j++)
                {
                    for (int i = 0 ; i < 5 && createdInstances < maxInstances; i++)
                    {
                        auto instance_E = game.entityManager.createEntity();
                        instance_E.addComponent<Transform>(&transform,
                            glm::vec3(rootLocation.x + j * 0.1f, rootLocation.y - i * 0.05, rootLocation.z + i * 0.01f),
                            0,
                            glm::vec3(rootScale.x, rootScale.y, rootScale.z));
                        instance_E.addComponent<UnitState>();

                        createdInstances++;
                    }
                }
            }
        });
    }

    void InstanceManager::terminate(engine::Game& game)
    {
        game.componentManager.forEachComponent<Transform>([&](Transform &transform)
        {
            if (!game.componentManager.hasComponent<Unit>(transform.entity())) [[likely]] return;
            for (auto child : transform.getChildTransforms())
            {
                auto& child_E = game.entityManager.getEntity(child->entity());
                game.entityManager.deleteEntity(child_E);
            }
        });
    }

    void InstanceManager::update(engine::Game& game)
    {
        int amount = 0;
        int amountLastFrame = 0;
        engine.componentManager.forEachComponent<InstanceBuffer>([&](InstanceBuffer& instanceBuffer) {
            auto iRootEntity = instanceBuffer.entity();
            if (!engine.componentManager.hasComponent<Transform>(iRootEntity)) return;

            auto pUnitRootTransform_C = &engine.componentManager.getComponent<Transform>(iRootEntity);

            if (engine.componentManager.hasComponent<Unit>(iRootEntity))
            {
                amount = engine.componentManager.getComponent<Unit>(iRootEntity).totalAmount;
                amountLastFrame = engine.componentManager.getComponent<Unit>(iRootEntity).totalAmountLastFrame;
                engine.componentManager.getComponent<Unit>(iRootEntity).totalAmountLastFrame = amount;;
            }

            instanceBuffer.instances.clear();
            for (auto pInstanceTransform: pUnitRootTransform_C->getChildTransforms())
            {
                if (amount < amountLastFrame)
                {
                    engine.entityManager.deleteEntity(engine.entityManager.getEntity(pInstanceTransform->entity()));
                    amountLastFrame--;
                }
                else if (amountLastFrame < amount)
                {

                }
                else
                {
                    instanceBuffer.instances.push_back(engine.calculateMvpMatrix(pInstanceTransform->modelMatrix));
                }
            }
            if (!engine.componentManager.hasComponent<UvOffset>(iRootEntity)) return;

            int totalFrames = 4;
            float frameDuration = 0.1f;
            int currentFrame = static_cast<int>(engine.elapsedTime / frameDuration) % totalFrames;

            auto& uvOffset_C = engine.componentManager.getComponent<UvOffset>(iRootEntity);
            uvOffset_C.value = currentFrame * (1.0f / totalFrames);
        });
    }
} // gl3