//
// Created by Julian Kalb on 19/07/2025.
//

#pragma once
#include <vector>
#include "../components/unitTypes/UnitCategory.h"
#include "../components/unitTypes/Unit.h"
#include "../components/unitTypes/SiegeEngine.h"


namespace gl3
{
    struct Option
    {
        Unit* actor;
        Unit* target;
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
        static float getCategoryPriority(UnitCategory category)
        {
            switch (category)
            {
            case UnitCategory::INFANTRY: return 1.0f;
            case UnitCategory::ARCHER: return 1.8f;
            case UnitCategory::SIEGE: return 1.5f;
            default: return 1.0f;
            }
        }
    Unit* pInfU_C = nullptr;
    Unit* pArcU_C = nullptr;
    Unit* pCatU_C = nullptr;

    Unit* eInfU_C = nullptr;
    Unit* eArcU_C = nullptr;
    Unit* eCatU_C = nullptr;
    };
}
