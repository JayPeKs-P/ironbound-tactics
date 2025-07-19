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
    namespace engine
    {
        class Game;
    }

    struct Option
    {
        Unit* actor;
        Unit* target;
        int amount;
        float score;
    };
    class ActionEvaluation {
    public:
        static void setPointers(engine::Game& game);
        static std::vector<Option> generateAttackOptions();
        static Unit* pInfU_C;
        static Unit* pArcU_C;
        static Unit* pCatU_C;
        static SiegeEngine* pCatSE_C;

        static Unit* eInfU_C;
        static Unit* eArcU_C;
        static Unit* eCatU_C;
        static SiegeEngine* eCatSE_C;
    private:
        static float getTargetHP(Unit* unit);
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
    };
}
