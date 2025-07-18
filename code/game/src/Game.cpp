//
// Created by Julian Kalb on 22/10/2024.
//

#include "Game.h"

#include <charconv>
#include <random>                           // for std::mt19937, std::uniform_int_distribution and std::random_device

#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Assets.h"

#include "components/TagComponent.h"
#include "components/GuiState.h"
#include "components/unitTypes/Catapult.h"
#include "components/unitTypes/Archer.h"
#include "components/unitTypes/Infantry.h"
#include "systems/GuiHandler.h"
#include "components/CombatSelection.h"
#include "gui/GuiCombat.h"
// #include "entities/Enemy.h"
// #include "entities/Planet.h"
#include "components/InstanceBuffer.h"
#include "components/Model2D.h"
#include "components/Shader.h"
#include "engine/Texture.h"
#include "engine/util/VertPresets.h"


using namespace gl3;

Game::Game(int width, int height, const std::string &title):
engine::Game(width, height, title)
{
    // audio.init();
    // audio.setGlobalVolume(0.1f);
}

void Game::start()
{
    unsigned int tempTexID = -1;
    //---- Entities ----
    auto &guiScene_E = engine::Game::entityManager.createEntity();
    guiScene_E.addComponent<GuiState>(GuiScene{GuiScene::COMBAT_MENU});
    guiScene_E.addComponent<CombatSelection<GuiCombat>>();

    auto &terrain_E = engine::Game::entityManager.createEntity();
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Terrain.png");
    terrain_E.addComponent<Model2D>(engine::util::VertPreset::background, engine::util::VertPreset::quadIndices, tempTexID);
    terrain_E.addComponent<Shader>();


    //----- Entities of player's army -----
    auto &pInf_E = engine::Game::entityManager.createEntity();
    auto &pInf_C = pInf_E.addComponent<Infantry>(0);
    pInf_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png");
    pInf_E.addComponent<Model2D>(engine::util::VertPreset::quad, engine::util::VertPreset::quadIndices, tempTexID);
    pInf_E.addComponent<InstanceBuffer>();
    pInf_E.addComponent<Shader>();
    //TODO add position Component

    auto &pArc_E = engine::Game::entityManager.createEntity();
    auto &pArc_C = pArc_E.addComponent<Archer>(0);
    pArc_E.addComponent<TagComponent>(Tag{Tag::PLAYER});

    auto &pCat_E = engine::Game::entityManager.createEntity();
    auto &pCat_C = pCat_E.addComponent<Catapult>(0);
    pCat_E.addComponent<TagComponent>(Tag{Tag::PLAYER});


    //----- Entities of enemy's army -----
    auto &eInf_E = engine::Game::entityManager.createEntity();
    auto &eInf_C = eInf_E.addComponent<Infantry>(0);
    eInf_E.addComponent<TagComponent>(Tag{Tag::ENEMY});

    auto &eArc_E = engine::Game::entityManager.createEntity();
    auto &eArc_C = eArc_E.addComponent<Archer>(0);
    eArc_E.addComponent<TagComponent>(Tag{Tag::ENEMY});

    auto &eCat_E = engine::Game::entityManager.createEntity();
    auto &eCat_C = eCat_E.addComponent<Catapult>(0);
    eCat_E.addComponent<TagComponent>(Tag{Tag::ENEMY});

    guiHandler = std::make_unique<GuiHandler>(*this);
    guiHandler->initialize(*this);
    renderSystem = new RenderSystem(*this);
    combatController = new CombatController(*this);
    combatController->init(*this);
    backgroundMusic = std::make_unique<SoLoud::Wav>();
    backgroundMusic->load(resolveAssetPath("audio/electronic-wave.mp3").string().c_str());
    backgroundMusic->setLooping(true);
    audio.playBackground(*backgroundMusic);
}

void Game::update(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    combatController->handleTurn();
    renderSystem->update(*this);
    elapsedTime += deltaTime;
}

void Game::draw()
{
    std::vector <InstanceData> instances;
    float yCoord = -1.25;
    float zCoord = 0.0f;
    for (int i = 0; i < 5; i++)
    {
        yCoord += 0.05f;
        zCoord -= 0.01f;
        float xCoord = -2.25;
        for (int j = 0; j < 10; j++)
        {
            xCoord += 0.1f;
            InstanceData instanceX;
            instanceX.model = calculateMvpMatrix({xCoord, yCoord ,zCoord}, 0, {0.25, 0.25, 1});
            instances.push_back(instanceX);
        }
        InstanceData instanceY;
        instanceY.model = calculateMvpMatrix({-2.25,yCoord ,zCoord}, 0, {0.25, 0.25, 1} );
        instances.push_back(instanceY);
    }
    std::vector <InstanceData> backgroundInstances;
    InstanceData backgroundData;
    backgroundData.model = calculateMvpMatrix({0,0,0}, 0, {2.75,2.75,1});
    backgroundInstances.push_back(backgroundData);
    //test end
    // test new renderer
    int totalFrames = 4;
    float frameDuration = 0.1f;
    int currentFrame = static_cast<int>(elapsedTime / frameDuration) % totalFrames;

    auto& model2DContainer = componentManager.getContainer<Model2D>();
    for (auto &[owner, _]: model2DContainer)
    {
        if (!componentManager.hasComponent<Shader>(owner)) continue;
        auto shaderProgram = componentManager.getComponent<Shader>(owner).get_shader_program();
        if (componentManager.hasComponent<InstanceBuffer>(owner))
        {
            auto& instanceBuffer_C = componentManager.getComponent<InstanceBuffer>(owner);
            instanceBuffer_C.instances = instances;
            instanceBuffer_C.uvOffset = currentFrame * (1.0f / totalFrames);
        }else
        {
        }
    }
    renderSystem->draw(*this);
}