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
#include "systems/GuiHandler.h"
#include "components/CombatSelection.h"
#include "gui/GuiCombat.h"
#include "components/InstanceBuffer.h"
#include "components/Model2D.h"
#include "components/Shader.h"
#include "components/UnitState.h"
#include "components/unitTypes/SiegeEngine.h"
#include "engine/Texture.h"
#include "engine/util/VertPresets.h"

#include "engine/util/Debug.h"

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
    combatSelection_C = &guiScene_E.addComponent<CombatSelection<GuiCombat>>();

    auto &terrain_E = engine::Game::entityManager.createEntity();
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Terrain.png");
    terrain_E.addComponent<Model2D>(engine::util::VertPreset::background, engine::util::VertPreset::quadIndices, tempTexID);
    terrain_E.addComponent<Shader>();
    terrain_E.addComponent<engine::sceneGraph::Transform>(origin, glm::vec3(0,0,-0.91), 0, glm::vec3(2.75,2.75,1));


    //----- Entities of player's army -----
    auto &pInf_E = engine::Game::entityManager.createEntity();
    auto &pInfU_C = pInf_E.addComponent<Unit>(UnitCategory::INFANTRY, 100, 30, 50, 99, 10, 0, 2);
    pInf_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png");
    pInf_E.addComponent<Model2D>(engine::util::VertPreset::pQuad, engine::util::VertPreset::quadIndices, tempTexID);
    pInf_E.addComponent<InstanceBuffer>();
    pInf_E.addComponent<Shader>();
    unitTransforms.push_back(&pInf_E.addComponent<Transform>(origin, glm::vec3(-2.25f, -1.0f ,0.0f), 0, glm::vec3(0.25, 0.25, 1)));

    auto &pArc_E = engine::Game::entityManager.createEntity();
    auto &pArcU_C = pArc_E.addComponent<Unit>(UnitCategory::ARCHER, 80, 20, 40, 85, 8, 0, 1);
    pArc_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png");
    pArc_E.addComponent<Model2D>(engine::util::VertPreset::pQuad, engine::util::VertPreset::quadIndices, tempTexID);
    pArc_E.addComponent<InstanceBuffer>();
    pArc_E.addComponent<Shader>();
    unitTransforms.push_back(&pArc_E.addComponent<Transform>(origin, glm::vec3(-2.25f, -0.25f ,0.0f), 0, glm::vec3(0.25, 0.25, 1)));

    auto &pCat_E = engine::Game::entityManager.createEntity();
    auto &pCatU_C = pCat_E.addComponent<Unit>(UnitCategory::CATAPULT, 400, 20, 150, 80, 12, 0, 3);
    auto &pCatSE_C = pCat_E.addComponent<SiegeEngine>(5);
    pCat_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png");
    pCat_E.addComponent<Model2D>(engine::util::VertPreset::pQuad, engine::util::VertPreset::quadIndices, tempTexID);
    pCat_E.addComponent<InstanceBuffer>();
    pCat_E.addComponent<Shader>();
    unitTransforms.push_back(&pCat_E.addComponent<Transform>(origin, glm::vec3(-2.25f, 0.5f ,0.0f), 0, glm::vec3(0.25, 0.25, 1)));


    //----- Entities of enemy's army -----
    auto &eInf_E = engine::Game::entityManager.createEntity();
    auto &eInfU_C = eInf_E.addComponent<Unit>(UnitCategory::INFANTRY, 100, 30, 50, 99, 10, 0, 2);
    eInf_E.addComponent<TagComponent>(Tag{Tag::ENEMY});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_03_Idle.png");
    eInf_E.addComponent<Model2D>(engine::util::VertPreset::eQuad, engine::util::VertPreset::quadIndices, tempTexID);
    eInf_E.addComponent<InstanceBuffer>();
    eInf_E.addComponent<Shader>();
    unitTransforms.push_back(&eInf_E.addComponent<Transform>(origin, glm::vec3(1.75f, -0.75f ,0.0f), 0, glm::vec3(0.25, 0.25, 1)));

    auto &eArc_E = engine::Game::entityManager.createEntity();
    auto &eArcU_C = eArc_E.addComponent<Unit>(UnitCategory::ARCHER, 80, 20, 40, 85, 8, 0, 1);
    eArc_E.addComponent<TagComponent>(Tag{Tag::ENEMY});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_03_Idle.png");
    eArc_E.addComponent<Model2D>(engine::util::VertPreset::eQuad, engine::util::VertPreset::quadIndices, tempTexID);
    eArc_E.addComponent<InstanceBuffer>();
    eArc_E.addComponent<Shader>();
    unitTransforms.push_back(&eArc_E.addComponent<Transform>(origin, glm::vec3(1.75f, 0.0f ,0.0f), 0, glm::vec3(0.25, 0.25, 1)));

    auto &eCat_E = engine::Game::entityManager.createEntity();
    auto &eCatU_C = eCat_E.addComponent<Unit>(UnitCategory::CATAPULT, 400, 20, 150, 80, 12, 0, 3);
    auto &eCatSE_C = eCat_E.addComponent<SiegeEngine>(5);
    eCat_E.addComponent<TagComponent>(Tag{Tag::ENEMY});
    tempTexID = engine::util::Texture::load("assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_03_Idle.png");
    eCat_E.addComponent<Model2D>(engine::util::VertPreset::eQuad, engine::util::VertPreset::quadIndices, tempTexID);
    eCat_E.addComponent<InstanceBuffer>();
    eCat_E.addComponent<Shader>();
    unitTransforms.push_back(&eCat_E.addComponent<Transform>(origin, glm::vec3(1.75f, 0.75f ,0.0f), 0, glm::vec3(0.25, 0.25, 1)));

    guiHandler = std::make_unique<GuiHandler>(*this);
    guiHandler->initialize(*this);

    renderSystem = new RenderSystem(*this);

    instanceManager = new InstanceManager(*this);

    movementSystem = new MovementSystem(*this);

    combatController = new CombatController(*this);
    combatController->init(*this);

    selectionSystem = new SelectionSystem(*this);

    backgroundMusic = std::make_unique<SoLoud::Wav>();
    backgroundMusic->load(resolveAssetPath("audio/electronic-wave.mp3").string().c_str());
    backgroundMusic->setLooping(true);
    audio.playBackground(*backgroundMusic);
}
struct MouseInput
{
    bool pressed = false;
    bool clicked = false;

    void update(GLFWwindow* window, int GLFW_BUTTON)
    {
        bool current = glfwGetMouseButton(window, GLFW_BUTTON) == GLFW_PRESS;
        clicked = (current && !pressed);
        pressed = current;
    }
};

MouseInput left;
MouseInput right;
void Game::update(GLFWwindow *window)
{
    left.update(window, GLFW_MOUSE_BUTTON_LEFT);
    if (left.clicked)
    {
        Transform* selection = nullptr;
        selection = selectionSystem->select(*this, 0.1f);
        if (selection != nullptr)
        {
            auto root = selection->getParent();
            auto entity = root->entity();
            if (componentManager.hasComponent<Unit>(entity))
            {
                auto &unit = componentManager.getComponent<Unit>(entity);
                DEBUG_LOG(
                    << unitCategory_to_string(unit.category)
                    );
                if (combatSelection_C->selectionOne == nullptr)
                {
                    combatSelection_C->selectionOne = std::make_shared<Unit>(unit);
                }else
                {
                    combatSelection_C->selectionTwo = std::make_shared<Unit>(unit);
                }
            }
        }
    }
    right.update(window, GLFW_MOUSE_BUTTON_RIGHT);
    if(right.clicked)
    {
        combatSelection_C->selectionOne = nullptr;
        combatSelection_C->selectionTwo = nullptr;
    }
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    combatController->handleTurn();
    movementSystem->moveTo(*this, origin->localPosition,deltaTime);
    for (auto& transform_C : unitTransforms)
    {
        instanceManager->update(*this, transform_C);
    }
    renderSystem->update(*this);
    elapsedTime += deltaTime;
}

void Game::draw()
{
    renderSystem->draw(*this);
}