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
#include "components/UnitState.h"
#include "engine/Texture.h"
#include "engine/util/VertPresets.h"

using gl3::engine::sceneGraph::Transform;
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
    terrain_E.addComponent<engine::sceneGraph::Transform>(origin, glm::vec3(0,0,-0.91), 0, glm::vec3(2.75,2.75,1));


    //----- Entities of player's army -----
    auto &pInf_E = engine::Game::entityManager.createEntity();
    auto &pInf_C = pInf_E.addComponent<Infantry>(0);
    pInf_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png");
    pInf_E.addComponent<Model2D>(engine::util::VertPreset::pQuad, engine::util::VertPreset::quadIndices, tempTexID);
    pInf_E.addComponent<InstanceBuffer>();
    pInf_E.addComponent<Shader>();
    pInf_O = &pInf_E.addComponent<Transform>(origin, glm::vec3(-2.25f, -1.0f ,0.0f), 0, glm::vec3(0.25, 0.25, 1));

    auto &pArc_E = engine::Game::entityManager.createEntity();
    auto &pArc_C = pArc_E.addComponent<Archer>(0);
    pArc_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png");
    pArc_E.addComponent<Model2D>(engine::util::VertPreset::pQuad, engine::util::VertPreset::quadIndices, tempTexID);
    pArc_E.addComponent<InstanceBuffer>();
    pArc_E.addComponent<Shader>();
    pArc_O = &pArc_E.addComponent<Transform>(origin, glm::vec3(-2.25f, -0.25f ,0.0f), 0, glm::vec3(0.25, 0.25, 1));

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
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_03_Idle.png");
    eArc_E.addComponent<Model2D>(engine::util::VertPreset::eQuad, engine::util::VertPreset::quadIndices, tempTexID);
    eArc_E.addComponent<InstanceBuffer>();
    eArc_E.addComponent<Shader>();
    eArc_O = &eArc_E.addComponent<Transform>(origin, glm::vec3(1.5f, 0.0f ,0.0f), 0, glm::vec3(0.25, 0.25, 1));

    auto &eCat_E = engine::Game::entityManager.createEntity();
    auto &eCat_C = eCat_E.addComponent<Catapult>(0);
    eCat_E.addComponent<TagComponent>(Tag{Tag::ENEMY});

    guiHandler = std::make_unique<GuiHandler>(*this);
    guiHandler->initialize(*this);

    renderSystem = new RenderSystem(*this);

    instanceManager = new InstanceManager(*this);

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
    instanceManager->update(*this, pInf_O);
    instanceManager->update(*this, pArc_O);
    instanceManager->update(*this, eArc_O);
    renderSystem->update(*this);
    elapsedTime += deltaTime;
}

void Game::draw()
{
    renderSystem->draw(*this);
}