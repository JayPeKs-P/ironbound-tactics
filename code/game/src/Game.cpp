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

#include "components/UnitContainer.h"
#include "entities/objectTypes/Catapult.h"
#include "entities/unitTypes/Archer.h"
#include "entities/unitTypes/Infantry.h"
#include "components/TagComponent.h"
#include "systems/GuiHandler.h"
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

    guiHandler = new GuiHandler(*this);
    combatController = new CombatController(*this);
    combatController->init();

    //----- Entities of player's army -----
    auto &pInfantry = engine::Game::entityManager.createEntity();
    auto &pInfContainer = pInfantry.addComponent<UnitContainer<Infantry>>();
    pInfantry.addComponent<TagComponent>(Tag{Tag::PLAYER});

    auto &pArchers = engine::Game::entityManager.createEntity();
    auto &pArcContainer = pArchers.addComponent<UnitContainer<Archer>>();
    pArchers.addComponent<TagComponent>(Tag{Tag::PLAYER});

    auto &pCatapults = engine::Game::entityManager.createEntity();
    auto &pCatContainer = pCatapults.addComponent<UnitContainer<Catapult>>();
    pCatapults.addComponent<TagComponent>(Tag{Tag::PLAYER});


    //----- Entities of enemy's army -----
    auto &eInfantry = engine::Game::entityManager.createEntity();
    auto &eInfContainer = eInfantry.addComponent<UnitContainer<Infantry>>();
    eInfantry.addComponent<TagComponent>(Tag{Tag::ENEMY});

    auto &eArchers = engine::Game::entityManager.createEntity();
    auto &eArcContainer = eArchers.addComponent<UnitContainer<Archer>>();
    eArchers.addComponent<TagComponent>(Tag{Tag::ENEMY});

    auto &eCatapults = engine::Game::entityManager.createEntity();
    auto &eCatContainer = eCatapults.addComponent<UnitContainer<Catapult>>();
    eCatapults.addComponent<TagComponent>(Tag{Tag::ENEMY});


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

    // for (const auto & entity : entities)
    // {
    //     entity->update(this, deltaTime);
    // }
    combatController->update();
}

void Game::draw()
{
    // for (const auto & entity : entities)
    // {
    //     entity->draw(this);
    // }
    combatController->draw();
}