//
// Created by Julian Kalb on 22/10/2024.
//
#pragma once
#include "engine/Game.h"


#include <soloud.h>
#include <soloud_wav.h>
#include "components/CombatSelection.h"



class GuiCombat;

namespace gl3
{
    class Game: public engine::Game {
    public:
        Game(int width, int height, const std::string& title);

    private:
        void start() override;
        void update(GLFWwindow *window) override;
        void draw() override;

        guid_t pInfID_E = -1;
        guid_t pArcID_E = -1;
        guid_t pCatID_E = -1;

        guid_t eInfID_E = -1;
        guid_t eArcID_E = -1;
        guid_t eCatID_E = -1;

        float zRotation = 0.0f;
        float rotationSpeed = 120.0f;
        float xTranslate = 0.0f;
        float yTranslate = 0.0f;
        float translationSpeed = 1.0f;
    };
}
