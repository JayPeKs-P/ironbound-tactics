//
// Created by Julian Kalb on 22/10/2024.
//
#pragma once
#include "engine/Game.h"
// #include "../src/entities/Entity.h"


#include <soloud.h>
#include <soloud_wav.h>

#include "../src/gui/CombatController.h"
#include "systems/GuiHandler.h"


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

        void start() override;
        void update(GLFWwindow *window) override;
        void draw() override;



        // std::vector<std::unique_ptr<Entity>> entities;

//        Ship* ship = nullptr;

        // BattleMenu* battleMenu = nullptr;

        SoLoud::Soloud audio;
        std::unique_ptr<SoLoud::Wav>(backgroundMusic);


        float zRotation = 0.0f;
        float rotationSpeed = 120.0f;
        float xTranslate = 0.0f;
        float yTranslate = 0.0f;
        float translationSpeed = 1.0f;



    };
}
