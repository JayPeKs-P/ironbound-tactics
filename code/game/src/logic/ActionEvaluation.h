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
        guid_t actor;
        guid_t target;
        int amount;
        float score;
    };
    class ActionEvaluation {
    public:
        static void setGuids(engine::Game& game);
        static std::vector<Option> generateOptions(engine::Game &game);

        static guid_t pInf_E;
        static guid_t pArc_E;
        static guid_t pCat_E;

        static guid_t eInf_E;
        static guid_t eArc_E;
        static guid_t eCat_E;
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
