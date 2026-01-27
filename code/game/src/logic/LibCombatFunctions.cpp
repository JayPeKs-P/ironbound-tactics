//
// Created by Julian Kalb on 19/07/2025.
//

#include "LibCombatFunctions.h"
#include "engine/Game.h"

namespace gl3 {
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
        float amountDead = fDamage / pUnit_C->hpValue;
        pUnit_C->totalAmount -= static_cast<int>(amountDead);
        if (pUnit_C->totalAmount <= 0) pUnit_C->totalAmount = 0;
        if (pUnit_C->availableAmount > pUnit_C->totalAmount) pUnit_C->availableAmount = pUnit_C->totalAmount;
        // float differenceAD = damage - armorValue;
        // armorValue -= damage/20;
        // if (differenceAD > 0)
        // {
        //    lifePoints -= differenceAD;
        // }
        // return lifePoints;
    }

    void LibCombatFunctions::UseSiegeEngine(int iAmountTarget, guid_t iUnit_ID, guid_t iSiegeEngine_ID) const {
        auto pUnitActor = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        auto pSiegeEngineTarget = &m_Game.componentManager.getComponent<SiegeEngine>(iSiegeEngine_ID);

        pSiegeEngineTarget->useableAmount += iAmountTarget; //TODO refactor to  unused amount
        pUnitActor->totalAmount -= iAmountTarget * pSiegeEngineTarget->cost;
        pUnitActor->m_iScheduledForUse = iAmountTarget * pSiegeEngineTarget->cost;
    }

    void LibCombatFunctions::ResetUnit(guid_t iUnit_ID, int iAmount) const {
        auto pUnit_C = &m_Game.componentManager.getComponent<Unit>(iUnit_ID);
        int iAmountAddBack = pUnit_C->availableAmount + iAmount;
        if (iAmountAddBack > pUnit_C->totalAmount) {
            pUnit_C->availableAmount = pUnit_C->totalAmount;
        }
        else {
            pUnit_C->availableAmount = iAmountAddBack;
        }
    }
} // gl3
