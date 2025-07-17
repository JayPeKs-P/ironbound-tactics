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
engine::Game(width, height, title),
shader("shaders/vertexShader.vert", "shaders/fragmentShader.frag"),
mesh({
                    //positions                  // colors                  // texture coords
                     0.25f,   0.25f,   0.0f,      1.0f,   0.0f,   0.0f,       0.25f,   0.5f,
                     0.25f,  -0.25f,   0.0f,      0.0f,   1.0f,   0.0f,       0.25f,   0.25f,
                    -0.25f,  -0.25f,   0.0f,      0.0f,   0.0f,   1.0f,       0.0f,   0.25f,
                    -0.25f,   0.25f,   0.0f,      1.0f,   1.0f,   0.0f,       0.0f,   0.5f
                 },
                 {0, 1, 3,
                  1, 2, 3},
                  engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_03_Idle.png")),
background({
                    //positions                  // colors                  // texture coords
                     1.0f,   0.574f,  -0.1f,      1.0f,   0.0f,   0.0f,       1.0f,   1.0f,     //top right
                     1.0f,  -0.574f,  -0.1f,      0.0f,   1.0f,   0.0f,       1.0f,   0.0f,     //bottom right
                    -1.0f,  -0.574f,  -0.1f,      0.0f,   0.0f,   1.0f,       0.0f,   0.0f,     //bottom left
                    -1.0f,   0.574f,  -0.1f,      1.0f,   1.0f,   0.0f,       0.0f,   1.0f      //top left
                 },
                 {0, 1, 3,
                  1, 2, 3},
                  engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Terrain.png"))
{
    // audio.init();
    // audio.setGlobalVolume(0.1f);
}

void Game::start()
{
    // unsigned int VAO;
    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);

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

    unsigned int tempTexID = -1;
    //---- Entities ----
    auto &guiSceneEntity = engine::Game::entityManager.createEntity();
    guiSceneEntity.addComponent<GuiState>(GuiScene{GuiScene::COMBAT_MENU});
    guiSceneEntity.addComponent<CombatSelection<GuiCombat>>();


    //----- Entities of player's army -----
    auto &pInf_E = engine::Game::entityManager.createEntity();
    auto &pInf_C = pInf_E.addComponent<Infantry>(0);
    pInf_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Terrain.png");
    pInf_E.addComponent<Model2D>(tempTexID, engine::util::VertPreset::quad, engine::util::VertPreset::quadIndices);
    pInf_E.addComponent<InstanceBuffer>();
    pInf_E.addComponent<Shader>();

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
            instanceX.modelMatrix = calculateMvpMatrix({xCoord, yCoord ,zCoord}, 0, {0.25, 0.25, 1});
            instances.push_back(instanceX);
        }
        InstanceData instanceY;
        instanceY.modelMatrix = calculateMvpMatrix({-2.25,yCoord ,zCoord}, 0, {0.25, 0.25, 1} );
        instances.push_back(instanceY);
    }
    std::vector <InstanceData> backgroundInstances;
    InstanceData backgroundData;
    backgroundData.modelMatrix = calculateMvpMatrix({0,0,0}, 0, {2.75,2.75,1});
    backgroundInstances.push_back(backgroundData);
    //test end
    auto mvpMatrix = calculateMvpMatrix({-2.25,-1.25,0}, 0, {0.25, 0.25, 1} );

    shader.use();
    shader.setFloat("uvOffset", 0.0f);
    background.update(backgroundInstances);
    background.draw(1);
    shader.use();
    // shader.setMatrix("mvp", mvpMatrix);
    shader.setMatrix("mvp", glm::mat4(1.0f));
    mesh.update(instances);
    int totalFrames = 4;
    float frameDuration = 0.2f;
    int currentFrame = static_cast<int>(elapsedTime / frameDuration) % totalFrames;
    float uvOffset = currentFrame * (1.0f / totalFrames);
    shader.setFloat("uvOffset", uvOffset);
    mesh.draw(instances.size());
}