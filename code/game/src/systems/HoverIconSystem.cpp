//
// Created by Julian Kalb on 01/02/2026.
//

#include "HoverIconSystem.h"

#include "MovementSystem.h"
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"
#include "../components/TagComponent.h"
#include "../components/Icon.h"
#include "engine/rendering/InstanceBuffer.h"
#include "engine/Game.h"
#include "engine/sceneGraph/Transform.h"

namespace gl3 {
    HoverIconSystem::HoverIconSystem(engine::Game& game):
    System(game){
        game.onUpdate.addListener([&](engine::Game& game)
        {
            update(game);
        });
        using namespace gl3::engine::sceneGraph;
        guid_t iIcon = engine::ecs::invalidID;
        game.componentManager.forEachComponent<Icon>([&](Icon& icon)
        {
            iIcon = icon.entity();
        });
        game.componentManager.forEachComponent<Unit>([&](Unit& unit)
        {
            auto iUnit_ID = unit.entity();
            auto& unitRootTransform = game.componentManager.getComponent<Transform>(iUnit_ID);
            auto iconRootTransform = &game.componentManager.getComponent<Transform>(iIcon);
            auto pIcon_E = &game.entityManager.createEntity();
            game.entityManager.SetParent(pIcon_E->guid(), iUnit_ID);
            glm::vec3 iconPosition = unitRootTransform.localPosition;
            iconPosition.y += 0.15f;
            iconPosition.z += 0.1f;
            pIcon_E->addComponent<Transform>(iconRootTransform, iconPosition, 0, glm::vec3(0.3f, 0.3f, 0.3f));
            pIcon_E->addComponent<UvOffset>(8, 2, 64, 64);
            pIcon_E->addComponent<Visibility>(false);
        });
    }

    void HoverIconSystem::update(engine::Game& game) {
        game.componentManager.forEachComponent<Unit>([&](Unit& unit)
        {
            auto pUnit_E = &game.entityManager.getEntity(unit.entity());
            auto pUnitInstanceBuffer_C = &game.componentManager.getComponent<InstanceBuffer>(pUnit_E->guid());
            for (auto child : pUnit_E->GetChildren()) {
                if (!game.componentManager.hasComponent<UvOffset>(child)) continue;
                auto pVisibility_C = &game.componentManager.getComponent<Visibility>(child);
                if (unit.totalAmount <= 0) {
                    pVisibility_C->m_bVisible = false;
                    continue;
                }
                if (pUnitInstanceBuffer_C->instances.empty()) {
                    pVisibility_C->m_bVisible = false;
                    continue;
                }

                pVisibility_C->m_bVisible = true;
            }
        });
    }
}
