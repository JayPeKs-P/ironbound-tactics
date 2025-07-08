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


using namespace gl3;

Game::Game(int width, int height, const std::string &title):
engine::Game(width, height, title)
{
    // audio.init();
    // audio.setGlobalVolume(0.1f);

}

void Game::start()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    std::mt19937 randomNumberEngine{       //Mersenne Twister generates 32-bit unsigned integers
        std::random_device{}()};       // Seed our Mersenne Twister using with a random number from the OS
    std::uniform_real_distribution positionDist{-2.0f,2.0f};
    std::uniform_real_distribution scaleDist{0.05f,0.2f};
    std::uniform_real_distribution colorDist{0.0f,1.0f};
    // for(auto i = 0; i < 100; ++i) {
    //     glm::vec3 randomPosition = {positionDist(randomNumberEngine) * 1.5f,
    //                                 positionDist(randomNumberEngine) * 1.5f,
    //                                 0.0f};
    //     float randomScale = scaleDist(randomNumberEngine);
    //     auto rgbValue = colorDist(randomNumberEngine);      //Used to guarantee, that all planets are a shade of grey.
    //     glm::vec4 randomColor = {rgbValue,
    //                              rgbValue,
    //                              rgbValue,
    //                              1.0};
    //
    //     auto entity = std::make_unique<Planet>(randomPosition, randomScale, randomColor);
    //     entities.push_back(std::move(entity));
    // }

    // auto spaceShip = std::make_unique<Ship>(glm::vec3(-2, 1, 0));
    // ship = spaceShip.get();
    // entities.push_back(std::move(spaceShip));
    //
    // auto enemy = std::make_unique<Enemy>(glm::vec3(2, -1, 0), -90.0f, 0.25f);
    // entities.push_back(std::move(enemy));


    //---- Entities ----
    auto &guiSceneEntity = engine::Game::entityManager.createEntity();
    guiSceneEntity.addComponent<GuiState>(GuiScene{GuiScene::COMBAT_MENU});
    guiSceneEntity.addComponent<CombatSelection<GuiCombat>>();


    //----- Entities of player's army -----
    auto &pInf_E = engine::Game::entityManager.createEntity();
    auto &pInf_C = pInf_E.addComponent<Infantry>(0);
    pInf_E.addComponent<TagComponent>(Tag{Tag::PLAYER});

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
}

void Game::draw()
{
    // for (const auto & entity : entities)
    // {
    //     entity->draw(this);
    // }
}