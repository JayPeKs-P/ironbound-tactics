//
// Created by Julian Kalb on 19/07/2025.
//

#pragma once
#include "../components/CombatSelection.h"
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"

struct Catapult;
struct Archer;
struct Infantry;

namespace gl3
{
    struct Option
    {
        Unit* actorUnit;
        SiegeEngine* actorSiege;
        Category targetType;
        int amount;
        float score;
    };
    class ActionEvaluation {
    public:
        std::vector<Option> generateAttackOptions();
        // static Infantry* pInf_C;
        // static Archer* pArc_C;
        // static Catapult* pCat_C;
        //
        // static Infantry* eInf_C;
        // static Archer* eArc_C;
        // static Catapult* eCat_C;
    private:
        float getTargetHP(Unit* unit);
        float getTargetHP(SiegeEngine* siege);
        static float getCategoryPriority(Category category)
        {
            switch (category)
            {
            case Category::INFANTRY: return 1.0f;
            case Category::ARCHER: return 1.8f;
            case Category::SIEGE: return 1.5f;
            default: return 1.0f;
            }
        }
    Infantry* pInf_C = nullptr;
    Archer* pArc_C = nullptr;
    Catapult* pCat_C = nullptr;

    Infantry* eInf_C = nullptr;
    Archer* eArc_C = nullptr;
    Catapult* eCat_C = nullptr;
    };
}
