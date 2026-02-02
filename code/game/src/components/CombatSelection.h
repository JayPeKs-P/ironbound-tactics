//
// Created by Julian Kalb on 07/07/2025.
//

#pragma once

#include <memory>

#include "engine/Events.h"
#include"unitTypes/UnitCategory.h"

struct Unit;
namespace gl3 {
    namespace engine {
        class Game;
    }

    struct CombatSelection {
        [[nodiscard]] static CombatSelection* GetInstance() {
            if (!m_pInstance) {
                m_pInstance = new CombatSelection();
            }
            return m_pInstance;
        }
        CombatSelection(CombatSelection const&) = delete;

        std::shared_ptr<Unit> m_pFirstUnit_C;
        std::shared_ptr<Unit> m_pSecondUnit_C;

        using event_t = engine::events::Event<CombatSelection, UnitCategory, int, UnitCategory>;
        void InvokeAttack(UnitCategory first, int iAmount, UnitCategory second) {
            attack.invoke(first, iAmount, second);
        }
        void InvokeDefend(UnitCategory first, int iAmount, UnitCategory second) {
            defend.invoke(first, iAmount, second);
        }
        void InvokeUse(UnitCategory first, int iAmount, UnitCategory second) {
            use.invoke(first, iAmount, second);
        }
        void ResetSelections() {
            m_pFirstUnit_C = nullptr;
            m_pSecondUnit_C = nullptr;
        }

        bool SetSelection(Unit* pSelection);
        event_t attack;
        event_t defend;
        event_t use;
        void HandleEdgeCases(engine::Game& game);

    private:
        explicit CombatSelection() {}
        static CombatSelection* m_pInstance;
    };
}