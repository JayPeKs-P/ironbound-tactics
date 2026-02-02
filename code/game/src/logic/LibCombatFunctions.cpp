//
// Created by Julian Kalb on 19/07/2025.
//

#include "LibCombatFunctions.h"

#include <complex.h>

#include "../components/TagComponent.h"
#include "engine/Game.h"
#include "engine/SoundSystem.h"
#define MAX_UNIT_AMOUNT 75

namespace gl3 {
    class TagComponent;
    LibCombatFunctions* LibCombatFunctions::m_pInstance = nullptr;

    void LibCombatFunctions::setAmount(guid_t iUnit_ID, int iValue) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        switch (pUnit_C->category) {
        case UnitCategory::CATAPULT:
            pUnit_C->totalAmount = iValue;
            pUnit_C->totalAmountLastFrame = iValue;
            pUnit_C->lifetimeMaxAmount = iValue;
            break;
        default:
            pUnit_C->totalAmount = iValue;
            pUnit_C->totalAmountLastFrame = iValue;
            pUnit_C->lifetimeMaxAmount = iValue;
            pUnit_C->availableAmount = iValue;
            break;
        }
    }

    float LibCombatFunctions::attack(guid_t iUnitActor_ID, int iAmount) const {
        auto pUnitActor_ID = &m_Game.componentManager.getComponent<Unit>(iUnitActor_ID);
        std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist{0, 99};
        float totalDamage = 0;
        for (int i = 0; i < iAmount; i++) {
            int hitRoll = dist(rng);
            if (hitRoll >= pUnitActor_ID->accuracy) {
                continue;
            }
            float damage = pUnitActor_ID->attackValue;

            int critRoll = dist(rng);
            if (critRoll < pUnitActor_ID->critChance) {
                damage *= pUnitActor_ID->critMultiplier;
            }
            totalDamage += damage;
        }
        return totalDamage;
    }

    void LibCombatFunctions::takeDamage(guid_t iUnit_ID, float fDamage) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);

        float fAmountDead = (fDamage - pUnit_C->armorValue * 0.5f) / pUnit_C->hpValue;
        int iAmountDead = static_cast<int>(fAmountDead);
        float fDifference = fAmountDead - static_cast<float>(iAmountDead);
        pUnit_C->fLeftoverDamage += fDifference;
        if (pUnit_C->fLeftoverDamage > 1.0f) {
            pUnit_C->fLeftoverDamage -= 1.0f;
            iAmountDead += 1;
        }
        if (iAmountDead <= 0) return;
        auto pSoundSystem = engine::SoundSystem::GetInstance();
        pSoundSystem->PlaySound(engine::DAMAGE_1);

        pUnit_C->totalAmount -= iAmountDead;
        if (pUnit_C->totalAmount <= 0) pUnit_C->totalAmount = 0;
        if (pUnit_C->availableAmount > pUnit_C->totalAmount) pUnit_C->availableAmount = pUnit_C->totalAmount;
        if (m_Game.componentManager.hasComponent<SiegeEngine>(iUnit_ID)) {
            auto pSiege_C = &m_Game.componentManager.getComponent<SiegeEngine>(iUnit_ID);
            pSiege_C->m_iUsedAmount -= iAmountDead;
            if (pSiege_C->m_iUsedAmount <= 0) pSiege_C->m_iUsedAmount = 0;
        }
    }

    void LibCombatFunctions::UseSiegeEngine(int iAmountTarget, guid_t iUnit_ID, guid_t iSiegeEngine_ID) const {
        auto pSoundSystem = engine::SoundSystem::GetInstance();
        pSoundSystem->PlaySound(engine::USE_1);
        auto pUnitActor = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        auto pSiegeEngineTarget = &m_Game.componentManager.getComponent<SiegeEngine>(iSiegeEngine_ID);

        // I moved this line to CombatController. Reason is, that I call this function in ActionRegister. This value
        // has to be decreased immediately though, since it is used in Gui
        // ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
        // pSiegeEngineTarget->useableAmount += iAmountTarget;
        pSiegeEngineTarget->m_iUsedAmount += iAmountTarget;
        pSiegeEngineTarget->m_iUsedAmountNew -= iAmountTarget;
        pUnitActor->totalAmount -= iAmountTarget * pSiegeEngineTarget->cost;
        pUnitActor->m_iScheduledForUse = iAmountTarget * pSiegeEngineTarget->cost;
    }

    void LibCombatFunctions::ResetUnit(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        int iAmountAddBack = pUnit_C->availableAmount + iAmount;
        int iCapReset = pUnit_C->totalAmount;

        if (iAmountAddBack > iCapReset) {
            pUnit_C->availableAmount = iCapReset;
        }
        else {
            pUnit_C->availableAmount = iAmountAddBack;
        }
    }

    void LibCombatFunctions::RegisterRewardFunctions() {
        m_FunctionKeys.resize(REWARD_LAST);
        m_FunctionKeys[ADD] = "Add";
        m_RewardCallbacks[m_FunctionKeys[ADD]] = &LibCombatFunctions::AddUnit;

        m_FunctionKeys[ATTACK] = "+ Attack";
        m_RewardCallbacks[m_FunctionKeys[ATTACK]] = &LibCombatFunctions::IncreaseAttack;

        m_FunctionKeys[DEFENSE] = "+ Defense";
        m_RewardCallbacks[m_FunctionKeys[DEFENSE]] = &LibCombatFunctions::IncreaseDefense;

        m_FunctionKeys[HEALTH] = "+ Base Health";
        m_RewardCallbacks[m_FunctionKeys[HEALTH]] = &LibCombatFunctions::IncreaseHealth;

        m_FunctionKeys[CRIT_CHANCE] = "+ Crit-Chance";
        m_RewardCallbacks[m_FunctionKeys[CRIT_CHANCE]] = &LibCombatFunctions::IncreaseCritChance;

        m_FunctionKeys[CRIT_MULT] = "+ Crit-Mult";
        m_RewardCallbacks[m_FunctionKeys[CRIT_MULT]] = &LibCombatFunctions::IncreaseCritMult;
    }

    void LibCombatFunctions::AddUnit(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        pUnit_C->totalAmount += iAmount;
        if (pUnit_C->lifetimeMaxAmount < pUnit_C->totalAmount) {
            pUnit_C->lifetimeMaxAmount = pUnit_C->totalAmount;
        }
        if (!m_Game.componentManager.hasComponent<SiegeEngine>(iUnit_ID)) {
            pUnit_C->availableAmount = pUnit_C->totalAmount;
        }
        else {
            auto pSiege_C = &m_Game.componentManager.getComponent<SiegeEngine>(iUnit_ID);
            pSiege_C->m_iUsedAmount += iAmount;
        }
    }

    void LibCombatFunctions::IncreaseAttack(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        pUnit_C->attackValue += static_cast<float>(iAmount);
    }

    void LibCombatFunctions::IncreaseDefense(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        pUnit_C->armorValue += static_cast<float>(iAmount);
    }

    void LibCombatFunctions::IncreaseHealth(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        pUnit_C->hpValue += static_cast<float>(iAmount);
        pUnit_C->fLeftoverDamage = 0;
    }

    void LibCombatFunctions::IncreaseCritChance(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        pUnit_C->critChance += iAmount;
    }

    void LibCombatFunctions::IncreaseCritMult(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        auto fFraction = static_cast<float>(iAmount) / 10.0f;
        pUnit_C->critMultiplier += fFraction;
    }

    RandomReward LibCombatFunctions::GetRandomReward() const {
        guid_t iInfantry_ID = engine::ecs::invalidID;
        guid_t iArcher_ID = engine::ecs::invalidID;
        guid_t iCatapult_ID = engine::ecs::invalidID;
        m_Game.componentManager.forEachComponent<Unit>([&](Unit &unit)
        {
            auto tag = &m_Game.componentManager.getComponent<TagComponent>(unit.entity());
            if (tag->value != Tag::PLAYER) return;
            switch (unit.category)
            {
            case UnitCategory::INFANTRY:
                iInfantry_ID = unit.entity();
                break;
            case UnitCategory::ARCHER:
                iArcher_ID = unit.entity();
                break;
            case UnitCategory::CATAPULT:
                iCatapult_ID = unit.entity();
                break;
            }
        });

        std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist{50, 150};
        int random = dist(rng);

        RandomReward result;

        guid_t iUnit_ID;
        int iAmount = random/10;

        assert(!m_FunctionKeys.empty());
        int iFunctionIndex = random % m_FunctionKeys.size();
        if (random % 3 == 0) {
            iUnit_ID = iInfantry_ID;
        }
        else if (random % 3 == 1) {
            iUnit_ID = iArcher_ID;
        }
        else {
            iUnit_ID = iCatapult_ID;
        }

        if (iUnit_ID == iCatapult_ID) {
            int iUnitBias = 12;
            int iBias = random % iUnitBias;
            if (iBias == 0 || iBias == 1) {
                iUnit_ID = iInfantry_ID;
            }
            else if (iBias == 2 || iBias == 3 || iBias == 4) {
                iUnit_ID = iArcher_ID;
            }
        }

        int iAddBias = 3;
        bool bAddBias = (random % iAddBias == 0);
        if (iFunctionIndex != ADD && bAddBias) {
            iFunctionIndex = ADD;
            iAmount =static_cast<int>(static_cast<float>(iAmount) * 1.5f) ;
        }


        if (iFunctionIndex == ADD) {
            auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
            if (pUnit_C->totalAmount + iAmount > MAX_UNIT_AMOUNT) {
                if ( MAX_UNIT_AMOUNT - pUnit_C->totalAmount > 0) {
                    iAmount = MAX_UNIT_AMOUNT - pUnit_C->totalAmount;
                }
                else {
                    iFunctionIndex += iUnit_ID + 1;
                }
            }
        }

        return {
            m_FunctionKeys[iFunctionIndex],
            iUnit_ID,
            iAmount};
    }

    void LibCombatFunctions::InvokeRewardCallback(RandomReward& reward) const {
        auto pCallback = m_RewardCallbacks.at(reward.m_FunctionKey);
        (this->*pCallback)(reward.m_iUnit_ID, reward.m_iAmount);
    }
} // gl3
