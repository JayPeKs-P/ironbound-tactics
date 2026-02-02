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
            auto pBounce = &pIcon_E->addComponent<Bounce>();
            pBounce->m_fBase = iconPosition.y;
        });
    }

    void HoverIconSystem::update(engine::Game& game) {
        bool bNewTurn = CombatController::getCombatState() == CombatState::BEGIN_TURN;
        game.componentManager.forEachComponent<Unit>([&](Unit& unit)
        {
            if (bNewTurn) {
                bool bIsSiege = game.componentManager.hasComponent<SiegeEngine>(unit.entity());
                unit.m_bShouldHaveIcon = unit.availableAmount > 0 || bIsSiege;
            }
            auto pUnit_E = &game.entityManager.getEntity(unit.entity());
            for (auto iIcon : pUnit_E->GetChildren()) {
                if (!game.componentManager.hasComponent<UvOffset>(iIcon)) continue;
                HandleIconLogic(unit, iIcon);
                AnimateIcon(iIcon);
            }
        });
    }

    void HoverIconSystem::AnimateIcon(guid_t iIcon) const {
        auto pTransform_C = &engine.componentManager.getComponent<Transform>(iIcon);
        auto pBounce_C = &engine.componentManager.getComponent<Bounce>(iIcon);
        float fDeltaTime = engine.getDeltaTime();
        pBounce_C->m_fCurrentTime += fDeltaTime * pBounce_C->m_fSpeed;

        float two_pi = glm::two_pi<float>();
        if (pBounce_C->m_fCurrentTime > two_pi) {
            pBounce_C->m_fCurrentTime -= two_pi;
        }
        pTransform_C->localPosition.y = pBounce_C->m_fBase + pBounce_C->m_fAmplitude * glm::sin(pBounce_C->m_fCurrentTime);
    }

    void HoverIconSystem::HandleIconLogic(Unit& unit, guid_t iIcon) const {
        auto pCombatSelection = CombatSelection::GetInstance();
        auto pVisibility_C = &engine.componentManager.getComponent<Visibility>(iIcon);

        if (HelperVisibilityBaseCases(unit, pVisibility_C)) {
            return;
        }
        auto pUvOffset_C = &engine.componentManager.getComponent<UvOffset>(iIcon);
        if (pCombatSelection->m_pSecondUnit_C) {
            if (HelperCasesSecondSelection(unit, pVisibility_C, pUvOffset_C)) return;
        }
        else if (pCombatSelection->m_pFirstUnit_C) {
            if (HelperCasesFirstSelection(unit, pVisibility_C, pUvOffset_C)) return;
        }
        else {
            pUvOffset_C->u = pUvOffset_C->originalU;
            pUvOffset_C->v = pUvOffset_C->originalV;
        }
        pVisibility_C->m_bVisible = true;
    }

    bool HoverIconSystem::HelperVisibilityBaseCases(Unit& unit, Visibility* pVisibility_C) const {
        const auto& cUnitInstanceBuffer_C = engine.componentManager.getComponent<InstanceBuffer>(unit.entity());
        if (CombatController::getCombatState() != CombatState::MAIN_PHASE) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        if (unit.totalAmount <= 0) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        if (cUnitInstanceBuffer_C.instances.empty()) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        if (!unit.m_bShouldHaveIcon) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        return false;
    }

    bool HoverIconSystem::HelperCasesSecondSelection(Unit& unit, Visibility* pVisibility_C, UvOffset* pUvOffset_C) const {
        auto pCombatSelection = CombatSelection::GetInstance();
        guid_t iSelection = pCombatSelection->m_pFirstUnit_C->entity();
        guid_t iSelection2 = pCombatSelection->m_pSecondUnit_C->entity();
        int iSelectionAvailable = pCombatSelection->m_pFirstUnit_C->availableAmount;
        auto pTag_C = &engine.componentManager.getComponent<TagComponent>(iSelection);
        auto pTag2_C = &engine.componentManager.getComponent<TagComponent>(iSelection2);

        bool bPlayerSelected = pTag_C->value == Tag::PLAYER;
        bool bEnemySelected2 = pTag2_C->value == Tag::ENEMY;
        bool bIsSelection1 = unit.entity() == iSelection;
        bool bIsSelection2 = unit.entity() == iSelection2;
        if (bPlayerSelected && !bIsSelection1 && !bIsSelection2) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        if (bIsSelection2 && bEnemySelected2 && iSelectionAvailable > 0) {
            pUvOffset_C->u = 8;
            pUvOffset_C->v = 48;
        }
        return false;
    }

    bool HoverIconSystem::HelperCasesFirstSelection(Unit& unit, Visibility* pVisibility_C, UvOffset* pUvOffset_C) const {
        auto pCombatSelection = CombatSelection::GetInstance();
        guid_t iSelection = pCombatSelection->m_pFirstUnit_C->entity();
        int iSelectionAvailable = pCombatSelection->m_pFirstUnit_C->availableAmount;
        auto pTag_C = &engine.componentManager.getComponent<TagComponent>(iSelection);

        bool bPlayerSelected = pTag_C->value == Tag::PLAYER;
        bool bUnitIsSiege = engine.componentManager.hasComponent<SiegeEngine>(unit.entity());
        bool bUnitIsEnemy = engine.componentManager.getComponent<TagComponent>(unit.entity()).value == Tag::ENEMY;
        bool bUnitIsSelection = iSelection == unit.entity();
        if (bPlayerSelected && !bUnitIsSiege && !bUnitIsEnemy && !bUnitIsSelection) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        if (!bPlayerSelected && !bUnitIsSelection) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        if (!bUnitIsSelection && iSelectionAvailable <= 0) {
            pVisibility_C->m_bVisible = false;
            return true;
        }
        bool bSiegeIsUseable = false;
        if (bUnitIsSiege && !bUnitIsEnemy) {
            auto& siege = engine.componentManager.getComponent<SiegeEngine>(unit.entity());
            auto iTotalSiege = unit.totalAmount;
            auto iTotalUsedAmount = siege.m_iUsedAmount + siege.m_iUsedAmountNew;;
            bool bHasUnused = iTotalSiege - iTotalUsedAmount > 0;
            bool bSelectionEnough = iSelectionAvailable > siege.cost;
            bSiegeIsUseable = bHasUnused && bSelectionEnough;
        }
        if (bUnitIsSelection) {
            pUvOffset_C->u = 20;
            pUvOffset_C->v = pUvOffset_C->originalV;
        }
        else if (!bUnitIsEnemy && bSiegeIsUseable) {
            pUvOffset_C->u = 8;
            pUvOffset_C->v = 49;
        }
        else if (!bUnitIsEnemy && !bSiegeIsUseable) {
            pUvOffset_C->u = 7;
            pUvOffset_C->v = 49;
        }
        else {
            pUvOffset_C->u = 11;
            pUvOffset_C->v = pUvOffset_C->originalV;
        }
        return false;
    }
}
