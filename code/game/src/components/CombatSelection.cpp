//
// Created by Julian Kalb on 02/02/2026.
//
#include "CombatSelection.h"

#include "TagComponent.h"
#include "unitTypes/Unit.h"
#include "engine/Game.h"
#include "unitTypes/SiegeEngine.h"

namespace gl3 {
    bool CombatSelection::SetSelection(Unit* pSelection) {
        if (pSelection == nullptr) return false;
        if (!m_pFirstUnit_C)
        {
            m_pFirstUnit_C = std::make_shared<Unit>(*pSelection);
        }
        else
        {
            m_pSecondUnit_C = std::make_shared<Unit>(*pSelection);
        }
        return true;
    }

    CombatSelection* CombatSelection::m_pInstance = nullptr;

    void CombatSelection::HandleEdgeCases(engine::Game& game) {
        if (!m_pFirstUnit_C || !m_pSecondUnit_C) return;
        auto& firstTag_C = game.componentManager.getComponent<TagComponent>(m_pFirstUnit_C->entity());
        auto& secondTag_C = game.componentManager.getComponent<TagComponent>(m_pSecondUnit_C->entity());
        bool bFirstIsPlayer = firstTag_C.value == Tag::PLAYER;
        bool bSecondIsPlayer = secondTag_C.value == Tag::PLAYER;

        bool bFirstIsSiege = game.componentManager.hasComponent<SiegeEngine>(m_pFirstUnit_C->entity());
        bool bSecondIsSiege = game.componentManager.hasComponent<SiegeEngine>(m_pSecondUnit_C->entity());

        if (!bFirstIsPlayer) {
            m_pFirstUnit_C = m_pSecondUnit_C;
            m_pSecondUnit_C = nullptr;
            return;
        }
        if (bSecondIsPlayer && !bSecondIsSiege) {
            m_pFirstUnit_C = m_pSecondUnit_C;
            m_pSecondUnit_C = nullptr;
            return;
        }
        if (m_pFirstUnit_C->availableAmount <= 0) {
            m_pFirstUnit_C = m_pSecondUnit_C;
            m_pSecondUnit_C = nullptr;
            return;
        }
        bool bSecondHasSiege = game.componentManager.hasComponent<SiegeEngine>(m_pSecondUnit_C->entity());
        if (!bSecondHasSiege) return;

        const auto& siege_C = game.componentManager.getComponent<SiegeEngine>(m_pSecondUnit_C->entity());
        int iTotalUsedAmount = siege_C.m_iUsedAmount + siege_C.m_iUsedAmountNew;
        bool bHasUnused = m_pSecondUnit_C->totalAmount - iTotalUsedAmount > 0;
        bool bFirstHasEnough = m_pFirstUnit_C->totalAmount > siege_C.cost;
        if (!bHasUnused || !bFirstHasEnough) {
            m_pFirstUnit_C = m_pSecondUnit_C;
            m_pSecondUnit_C = nullptr;
            return;
        }
    }
}
