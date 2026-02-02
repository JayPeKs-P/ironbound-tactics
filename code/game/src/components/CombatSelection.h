//
// Created by Julian Kalb on 07/07/2025.
//

#pragma once

#include <memory>

#include "engine/Events.h"
#include"unitTypes/UnitCategory.h"

struct Unit;
namespace gl3 {
    struct CombatSelection {
        [[nodiscard]] static CombatSelection* GetInstance() {
            if (!m_pInstance) {
                m_pInstance = new CombatSelection();
            }
            return m_pInstance;
        }
        CombatSelection(CombatSelection const&) = delete;

        std::shared_ptr<Unit> selectionOne;
        std::shared_ptr<Unit> selectionTwo;

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
        event_t attack;
        event_t defend;
        event_t use;

    private:
        explicit CombatSelection() {}
        static CombatSelection* m_pInstance;
    };
}