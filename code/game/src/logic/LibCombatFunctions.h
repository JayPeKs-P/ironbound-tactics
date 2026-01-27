//
// Created by Julian Kalb on 19/07/2025.
//

#pragma once
#include <random>
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"


namespace gl3 {
    namespace engine {
        class Game;
    }

    class LibCombatFunctions {
public:
    LibCombatFunctions(const LibCombatFunctions&obj) = delete;
    static LibCombatFunctions* GetInstance(engine::Game& game) {
        if (!m_pInstance) {
            m_pInstance = new LibCombatFunctions(game);
        }
        return m_pInstance;
    }

    void setAmount(guid_t iUnit_ID, int iValue) const;
    float attack(guid_t iUnitActor_ID, int iAmount) const;
    void takeDamage(guid_t iUnit_ID, float fDamage) const;
    void UseSiegeEngine(int iAmountTarget, guid_t pUnitActor, guid_t pSiegeEngineTarget) const;
    void ResetUnit(guid_t iUnit_ID, int iAmount) const;
private:
    LibCombatFunctions(engine::Game& game):
        m_Game(game){}
    static LibCombatFunctions* m_pInstance;
    engine::Game& m_Game;


};

} // gl3