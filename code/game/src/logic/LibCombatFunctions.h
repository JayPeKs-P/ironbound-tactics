//
// Created by Julian Kalb on 19/07/2025.
//

#pragma once
#include <random>
#include <unordered_map>

#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"


namespace gl3 {
    namespace engine {
        class Game;
    }
    struct RandomReward {
        std::string m_FunctionKey;
        guid_t m_iUnit_ID;
        int m_iAmount;
    };
    enum RandomRewardType {ADD, ATTACK, DEFENSE, /*SPEED,*/ CRIT_CHANCE, CRIT_MULT, REWARD_LAST};

    class LibCombatFunctions {
    public:
        LibCombatFunctions(const LibCombatFunctions& obj) = delete;

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

        // Reward Functions
        void AddUnit(guid_t iUnit_ID, int iAmount) const;
        void IncreaseAttack(guid_t iUnit_ID, int iAmount) const;
        void IncreaseDefense(guid_t iUnit_ID, int iAmount) const;
        void IncreaseSpeed(guid_t iUnit_ID, int iAmount) const;
        void IncreaseCritChance(guid_t iUnit_ID, int iAmount) const;
        void IncreaseCritMult(guid_t iUnit_ID, int iAmount) const;
        using pRewardFunction = void (LibCombatFunctions::*)(guid_t iUnitActor_ID, int iAmount) const;
        RandomReward GetRandomReward() const;
        void InvokeRewardCallback(RandomReward& reward) const;

    private:
        LibCombatFunctions(engine::Game& game) : m_Game(game) {
            RegisterRewardFunctions();
        }
        void RegisterRewardFunctions();

        static LibCombatFunctions* m_pInstance;
        engine::Game& m_Game;
        std::vector<std::string> m_FunctionKeys;

        std::unordered_map<std::string, pRewardFunction> m_RewardCallbacks;
    };
} // gl3
