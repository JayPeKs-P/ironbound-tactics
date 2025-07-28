//
// Created by Julian Kalb on 22/10/2024.
//
#pragma once
#include "engine/Game.h"


#include <soloud.h>
#include <soloud_wav.h>
#include "components/CombatSelection.h"

#include "systems/CombatController.h"
#include "systems/GuiHandler.h"
#include "systems/MovementSystem.h"
#include "systems/SelectionSystem.h"
#include "systems/rendering/InstanceManager.h"


class GuiCombat;

namespace gl3
{
    class Game: public engine::Game {
    public:
        Game(int width, int height, const std::string& title);

        [[nodiscard]] std::shared_ptr<GuiHandler> gui_handler() const
        {
            return guiHandler;
        }

    private:
        std::shared_ptr<GuiHandler> guiHandler;
        CombatController* combatController = nullptr;
        InstanceManager* instanceManager = nullptr;
        SelectionSystem* selectionSystem = nullptr;
        MovementSystem* movementSystem = nullptr;

        CombatSelection<GuiCombat>* combatSelection_C = nullptr;

        void start() override;
        void update(GLFWwindow *window) override;
        void draw() override;



        // std::vector<std::unique_ptr<Entity>> entities;

//        Ship* ship = nullptr;

        // BattleMenu* battleMenu = nullptr;

        // SoLoud::Soloud audio;
        // std::unique_ptr<SoLoud::Wav>(backgroundMusic);

        guid_t pInfID_E = -1;
        guid_t pArcID_E = -1;
        guid_t pCatID_E = -1;

        guid_t eInfID_E = -1;
        guid_t eArcID_E = -1;
        guid_t eCatID_E = -1;

        std::vector<engine::sceneGraph::Transform*> unitTransforms;

        float zRotation = 0.0f;
        float rotationSpeed = 120.0f;
        float xTranslate = 0.0f;
        float yTranslate = 0.0f;
        float translationSpeed = 1.0f;



    };
}
