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
        guid_t iActor_ID;
        guid_t iTarget_ID;
        int iTargetAmount;
        float score;
    };
    class EnemyAI {
    public:
        EnemyAI(const EnemyAI& obj) = delete;
        static EnemyAI* GetInstance() {
            if (!m_pInstance) {
                m_pInstance = new EnemyAI();
            }
            return m_pInstance;
        }

        void setGuids(engine::Game& game);
        std::vector<Option> generateOptions(engine::Game &game);

        guid_t iInfantryPlayer_ID;
        guid_t iArcherPlayer_ID;
        guid_t iCatapultPlayer_ID;

        guid_t iInfantryEnemy_ID;
        guid_t iArcherEnemy_ID;
        guid_t iCatapultEnemy_ID;
    private:
        EnemyAI() {}
        static EnemyAI* m_pInstance;
        float getTargetHP(Unit* unit);
        float getCategoryPriority(UnitCategory category)
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
