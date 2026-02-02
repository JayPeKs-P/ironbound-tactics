//
// Created by Julian Kalb on 01/02/2026.
//

#include "HoverIconSystem.h"

#include "CombatController.h"
#include "MovementSystem.h"
#include "../components/CombatSelection.h"
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
        auto pCombatSelection = CombatSelection::GetInstance();
        bool bNewTurn = CombatController::getCombatState() == CombatState::BEGIN_TURN;
        game.componentManager.forEachComponent<Unit>([&](Unit& unit)
        {
            if (bNewTurn) {
                bool bIsSiege = game.componentManager.hasComponent<SiegeEngine>(unit.entity());
                unit.m_bShouldHaveIcon = unit.availableAmount > 0 || bIsSiege;
            }
            auto pUnit_E = &game.entityManager.getEntity(unit.entity());
            auto pUnitInstanceBuffer_C = &game.componentManager.getComponent<InstanceBuffer>(pUnit_E->guid());
            for (auto iIcon : pUnit_E->GetChildren()) {
                if (!game.componentManager.hasComponent<UvOffset>(iIcon)) continue;
                auto pUvOffset_C = &game.componentManager.getComponent<UvOffset>(iIcon);

                auto pVisibility_C = &game.componentManager.getComponent<Visibility>(iIcon);
                if (CombatController::getCombatState() != CombatState::MAIN_PHASE) {
                    pVisibility_C->m_bVisible = false;
                    continue;
                }
                if (unit.totalAmount <= 0) {
                    pVisibility_C->m_bVisible = false;
                    continue;
                }
                if (pUnitInstanceBuffer_C->instances.empty()) {
                    pVisibility_C->m_bVisible = false;
                    continue;
                }
                if (!unit.m_bShouldHaveIcon) {
                    pVisibility_C->m_bVisible = false;
                    continue;
                }

                if (pCombatSelection->m_pSecondUnit_C) {
                    guid_t iSelection = pCombatSelection->m_pFirstUnit_C->entity();
                    guid_t iSelection2 = pCombatSelection->m_pSecondUnit_C->entity();
                    int iSelectionAvailable = pCombatSelection->m_pFirstUnit_C->availableAmount;
                    auto pTag_C = &game.componentManager.getComponent<TagComponent>(iSelection);
                    auto pTag2_C = &game.componentManager.getComponent<TagComponent>(iSelection2);

                    bool bPlayerSelected = pTag_C->value == Tag::PLAYER;
                    bool bEnemySelected2 = pTag2_C->value == Tag::ENEMY;
                    bool bIsSelection1 = unit.entity() == iSelection;
                    bool bIsSelection2 = unit.entity() == iSelection2;
                    if (bPlayerSelected && !bIsSelection1 && !bIsSelection2) {
                        pVisibility_C->m_bVisible = false;
                        continue;
                    }
                    if (bIsSelection2 && bEnemySelected2 && iSelectionAvailable > 0) {
                        pUvOffset_C->u = 8;
                        pUvOffset_C->v = 48;
                    }
                }
                else if (pCombatSelection->m_pFirstUnit_C) {
                    guid_t iSelection = pCombatSelection->m_pFirstUnit_C->entity();
                    int iSelectionAvailable = pCombatSelection->m_pFirstUnit_C->availableAmount;
                    auto pTag_C = &game.componentManager.getComponent<TagComponent>(iSelection);

                    bool bPlayerSelected = pTag_C->value == Tag::PLAYER;
                    bool bUnitIsSiege = game.componentManager.hasComponent<SiegeEngine>(pUnit_E->guid());
                    bool bUnitIsEnemy = game.componentManager.getComponent<TagComponent>(pUnit_E->guid()).value == Tag::ENEMY;
                    bool bUnitIsSelection = iSelection == unit.entity();
                    if (bPlayerSelected && !bUnitIsSiege && !bUnitIsEnemy && !bUnitIsSelection) {
                        pVisibility_C->m_bVisible = false;
                        continue;
                    }
                    if (!bUnitIsSelection && iSelectionAvailable <= 0) {
                        pVisibility_C->m_bVisible = false;
                        continue;
                    }
                    bool bSiegeIsUseable = false;
                    if (bUnitIsSiege && !bUnitIsEnemy) {
                        auto& siege = game.componentManager.getComponent<SiegeEngine>(pUnit_E->guid());
                        auto iTotalSiege = unit.totalAmount;
                        auto iUseableAmount = siege.useableAmount;
                        auto iUseAmountNew = siege.useableAmountNew;
                        auto iCost = siege.cost;
                        bool bHasUnused = iTotalSiege - iUseableAmount - iUseAmountNew > 0;
                        bool bSelectionEnough = iSelectionAvailable > iCost;
                        bSiegeIsUseable = bHasUnused && bSelectionEnough;
                    }
                    if (bUnitIsSelection || bSiegeIsUseable) {
                        pUvOffset_C->u = 20;
                    }
                    // else if (bSiegeIsUseable) {
                    //     pUvOffset_C->u = 8;
                    //     pUvOffset_C->v = 49;
                    // }
                    else {
                        pUvOffset_C->u = 11;
                    }
                }
                else {
                    pUvOffset_C->u = pUvOffset_C->originalU;
                    pUvOffset_C->v = pUvOffset_C->originalV;
                }

                pVisibility_C->m_bVisible = true;
            }
        });
    }

    void HoverIconSystem::HelperNoSelection(guid_t iUnit) {

    }
}
