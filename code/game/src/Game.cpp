//
// Created by Julian Kalb on 22/10/2024.
//

#include "Game.h"

#include <charconv>
#include <random>                           // for std::mt19937, std::uniform_int_distribution and std::random_device

#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/util/Assets.h"

#include "components/TagComponent.h"
#include "systems/GuiHandler.h"
#include "components/CombatSelection.h"
#include "components/Icon.h"
#include "gui/GuiCombat.h"
#include "engine/rendering/InstanceBuffer.h"
#include "engine/rendering/Shader.h"
#include "engine/rendering/Model2D.h"
#include "components/UnitState.h"
#include "components/unitTypes/SiegeEngine.h"
#include "components/unitTypes/Unit.h"
#include "engine/Texture.h"
#include "engine/util/VertPresets.h"

#include "engine/util/Debug.h"
#include "systems/CombatController.h"
#include "systems/MovementSystem.h"
#include "systems/SelectionSystem.h"
#include "systems/rendering/InstanceManager.h"
#include "engine/SoundSystem.h"
#include "systems/HoverIconSystem.h"

using gl3::engine::sceneGraph::Transform;
using namespace gl3;

Game::Game(int width, int height, const std::string& title):
    engine::Game(width, height, title) {
    auto pSoundSystem = engine::SoundSystem::GetInstance();
    pSoundSystem->RegisterSound(engine::UI_BUTTON_PRESS, "retro_ui_menu_simple_click_12.wav");
    pSoundSystem->RegisterSound(engine::UI_BUTTON_HOVER, "retro_ui_menu_simple_click_03.wav");
    pSoundSystem->RegisterSound(engine::UI_DRAG_SLIDER, "retro_ui_menu_blip_click_20.wav");

    // pSoundSystem->RegisterSound(, "retro_footstep_movement_05.wav");
    pSoundSystem->RegisterSound(engine::WALKING_1, "retro_move_walk_tick_20.wav");
    // pSoundSystem->RegisterSound(, "retro_impact_hit_general_33.wav");
    // pSoundSystem->RegisterSound(engine::USE_1, "retro_electric_zaps_spark_03.wav");
    pSoundSystem->RegisterSound(engine::USE_1, "retro_powerup_collect_24.wav");
    pSoundSystem->RegisterSound(engine::ADVANCE_1, "retro_powerup_item_05.wav");

    pSoundSystem->RegisterSound(engine::IMPACT_1, "retro_impact_colorful_01.wav", false);
    pSoundSystem->RegisterSound(engine::DAMAGE_1, "retro_damage_hurt_ouch_50.wav");

    pSoundSystem->RegisterSound(engine::VICTORY_1, "retro_beeps_collect_item_01.wav");
    pSoundSystem->RegisterSound(engine::L0SS_1, "retro_fail_sound_04.wav");
    pSoundSystem->RegisterMusic(engine::MUSIC_SELECTION, "Retro Action Game Theme #6 (looped).wav");
    pSoundSystem->RegisterMusic(engine::MUSIC_COMBAT_1, "Retro Action Game Theme #8 (looped).wav");
    pSoundSystem->RegisterMusic(engine::MUSIC_MAIN_MENU, "Logo #2 (Looped).wav");

}

void Game::start() {
    AddTextureToRegistry("assets/textures/entities/Tactical RPG overworld pack 3x/Terrain.png", "Terrain");
    AddTextureToRegistry("assets/textures/entities/Arrow_No_BG.png", "Arrow");
    AddTextureToRegistry("assets/textures/gui/ui_atlas_48x48.png", "GUI");
    unsigned int tempTexID = -1;

    using namespace engine::util;
    //---- Entities ----
    auto& guiScene_E = engine::Game::entityManager.createEntity();

    auto& terrain_E = engine::Game::entityManager.createEntity();
    tempTexID = GetTextureFromRegistry("Terrain");
    terrain_E.addComponent<Model2D>(VertPreset::background, VertPreset::quadIndices, tempTexID);
    terrain_E.addComponent<Shader>();
    terrain_E.addComponent<Transform>(origin, glm::vec3(0, 0, -0.91), 0, glm::vec3(2.75, 2.75, 1));

    //----- Boilerplate Entities -----
    auto pArrowPlayer_E = &entityManager.createEntity();
    int iArrowID = pArrowPlayer_E->guid();
    pArrowPlayer_E->addComponent<Projectile>(iArrowID);
    tempTexID = GetTextureFromRegistry("Arrow");
    pArrowPlayer_E->addComponent<Model2D>(VertPreset::QuadInanimate, VertPreset::quadIndices, tempTexID);
    pArrowPlayer_E->addComponent<Shader>();
    pArrowPlayer_E->addComponent<Transform>(origin);
    pArrowPlayer_E->addComponent<InstanceBuffer>();
    pArrowPlayer_E->addComponent<InstanceAmount>();
    // pArrowPlayer_E->addComponent<TagComponent>(Tag{Tag::PLAYER});

    auto pRootIcon_E = &entityManager.createEntity();
    pRootIcon_E->addComponent<Icon>();
    tempTexID = GetTextureFromRegistry("GUI");
    pRootIcon_E->addComponent<Model2D>(VertPreset::QuadInanimate, VertPreset::quadIndices, tempTexID);
    pRootIcon_E->addComponent<UvOffset>(9, 3, 64, 64);
    pRootIcon_E->addComponent<Shader>();
    pRootIcon_E->addComponent<Transform>(origin);
    pRootIcon_E->addComponent<InstanceBuffer>();


    // auto pArrowEnemy_E = &entityManager.createEntity();
    // iArrowID = pArrowEnemy_E->guid();
    // pArrowEnemy_E->addComponent<Projectile>(iArrowID);
    // tempTexID = GetTextureFromRegistry("Arrow");
    // pArrowEnemy_E->addComponent<Model2D>(VertPreset::QuadInanimate, VertPreset::quadIndices, tempTexID);
    // pArrowEnemy_E->addComponent<Shader>();
    // pArrowEnemy_E->addComponent<Transform>(origin);
    // pArrowEnemy_E->addComponent<InstanceBuffer>();
    // pArrowEnemy_E->addComponent<InstanceAmount>();
    // pArrowEnemy_E->addComponent<TagComponent>(Tag{Tag::ENEMY});

    //----- Entities of player's army -----
    auto& InfPlayer_E = engine::Game::entityManager.createEntity();
    pInfID_E = InfPlayer_E.guid();
    auto& pInfU_C = InfPlayer_E.addComponent<Unit>("pInfantry");
    InfPlayer_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    std::string texture = GetConfigEntry("InfantryTexture");
    tempTexID = Texture::load(texture.c_str());
    InfPlayer_E.addComponent<Model2D>(VertPreset::pQuad, VertPreset::quadIndices, tempTexID);
    InfPlayer_E.addComponent<AnimationSpeed>(0.1f);
    InfPlayer_E.addComponent<InstanceBuffer>();
    InfPlayer_E.addComponent<UvOffset>(PLAYER, 1, 4, 4);
    InfPlayer_E.addComponent<Shader>();
    &InfPlayer_E.addComponent<Transform>(origin,
        glm::vec3(-2.25f, -1.0f, 0.2f), 0, glm::vec3(0.25, 0.25, 1));

    auto& ArcPlayer_E = engine::Game::entityManager.createEntity();
    pArcID_E = ArcPlayer_E.guid();
    auto& pArcU_C = ArcPlayer_E.addComponent<Unit>("pArcher");
    ArcPlayer_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    texture = GetConfigEntry("ArcherTexture");
    tempTexID = Texture::load(texture.c_str());
    ArcPlayer_E.addComponent<Model2D>(VertPreset::pQuad, VertPreset::quadIndices, tempTexID);
    ArcPlayer_E.addComponent<AnimationSpeed>(0.1f);
    ArcPlayer_E.addComponent<InstanceBuffer>();
    ArcPlayer_E.addComponent<UvOffset>(PLAYER, 1, 4, 4);
    ArcPlayer_E.addComponent<Shader>();
    &ArcPlayer_E.addComponent<Transform>(origin,
        glm::vec3(-2.25f, -0.25f, 0.1f), 0, glm::vec3(0.25, 0.25, 1));

    auto& CatPlayer_E = engine::Game::entityManager.createEntity();
    pCatID_E = CatPlayer_E.guid();
    auto& pCatU_C = CatPlayer_E.addComponent<Unit>("pCatapult");
    auto& pCatSE_C = CatPlayer_E.addComponent<SiegeEngine>(5);
    CatPlayer_E.addComponent<TagComponent>(Tag{Tag::PLAYER});
    texture = GetConfigEntry("CatapultTexture");
    tempTexID = Texture::load(texture.c_str());
    CatPlayer_E.addComponent<Model2D>(engine::util::VertPreset::pQuad, engine::util::VertPreset::quadIndices, tempTexID);
    CatPlayer_E.addComponent<AnimationSpeed>(0.1f);
    CatPlayer_E.addComponent<InstanceBuffer>();
    CatPlayer_E.addComponent<UvOffset>(PLAYER, 1, 4, 4);
    CatPlayer_E.addComponent<Shader>();
    &CatPlayer_E.addComponent<Transform>(origin,
        glm::vec3(-2.25f, 0.5f, 0.0f), 0, glm::vec3(0.25, 0.25, 1));


    //----- Entities of enemy's army -----
    auto& InfEnemy_E = engine::Game::entityManager.createEntity();
    eInfID_E = InfEnemy_E.guid();
    auto& eInfU_C = InfEnemy_E.addComponent<Unit>(
        "pInfantry"
    );
    InfEnemy_E.addComponent<TagComponent>(Tag{Tag::ENEMY});
    InfEnemy_E.addComponent<Model2D>(engine::util::VertPreset::eQuad, engine::util::VertPreset::quadIndices, tempTexID);
    InfEnemy_E.addComponent<AnimationSpeed>(0.1f);
    InfEnemy_E.addComponent<InstanceBuffer>();
    InfEnemy_E.addComponent<UvOffset>(ENEMY, 1, 4, 4);
    InfEnemy_E.addComponent<Shader>();
    &InfEnemy_E.addComponent<Transform>(origin,
        glm::vec3(1.75f, -0.75f, 0.2f), 0, glm::vec3(0.25, 0.25, 1));

    auto& ArcEnemy_E = engine::Game::entityManager.createEntity();
    eArcID_E = ArcEnemy_E.guid();
    auto& eArcU_C = ArcEnemy_E.addComponent<Unit>(
        "pArcher"
    );
    ArcEnemy_E.addComponent<TagComponent>(Tag{Tag::ENEMY});
    ArcEnemy_E.addComponent<Model2D>(engine::util::VertPreset::eQuad, engine::util::VertPreset::quadIndices, tempTexID);
    ArcEnemy_E.addComponent<AnimationSpeed>(0.1f);
    ArcEnemy_E.addComponent<InstanceBuffer>();
    ArcEnemy_E.addComponent<UvOffset>(ENEMY, 1, 4, 4);
    ArcEnemy_E.addComponent<Shader>();
    &ArcEnemy_E.addComponent<Transform>(origin,
        glm::vec3(1.75f, 0.0f, 0.1f), 0, glm::vec3(0.25, 0.25, 1));

    auto& CatEnemy_E = engine::Game::entityManager.createEntity();
    eCatID_E = CatEnemy_E.guid();
    auto& eCatU_C = CatEnemy_E.addComponent<Unit>(
        "pCatapult"
    );
    auto& eCatSE_C = CatEnemy_E.addComponent<SiegeEngine>(5);
    CatEnemy_E.addComponent<TagComponent>(Tag{Tag::ENEMY});
    CatEnemy_E.addComponent<Model2D>(engine::util::VertPreset::eQuad, engine::util::VertPreset::quadIndices, tempTexID);
    CatEnemy_E.addComponent<AnimationSpeed>(0.1f);
    CatEnemy_E.addComponent<InstanceBuffer>();
    CatEnemy_E.addComponent<UvOffset>(ENEMY, 1, 4, 4);
    CatEnemy_E.addComponent<Shader>();
    &CatEnemy_E.addComponent<Transform>(origin,
        glm::vec3(1.75f, 0.75f, 0.0f), 0, glm::vec3(0.25, 0.25, 1));

    auto guiHandler = &addSystem<GuiHandler>();
    auto instanceManager = &addSystem<InstanceManager>();
    auto movementSystem = &addSystem<MovementSystem>();
    auto combatController = &addSystem<CombatController>();
    auto selectionSystem = &addSystem<SelectionSystem>();
    auto pHoverIconSystem = &addSystem<HoverIconSystem>();
    CombatController::initialize.addListener([&]()
    {
        auto pInfEnemyM2D_C = &componentManager.getComponent<Model2D>(eInfID_E);
        auto pArcEnemyM2D_C = &componentManager.getComponent<Model2D>(eArcID_E);
        auto pCatEnemyM2D_C = &componentManager.getComponent<Model2D>(eCatID_E);

        auto pInfEnemyU_C = &componentManager.getComponent<Unit>(eInfID_E);
        auto pArcEnemyU_C = &componentManager.getComponent<Unit>(eArcID_E);
        auto pCatEnemyU_C = &componentManager.getComponent<Unit>(eCatID_E);

        pInfEnemyM2D_C->texture = engine::util::Texture::load(pInfEnemyU_C->texturePath.c_str());
        pArcEnemyM2D_C->texture = engine::util::Texture::load(pArcEnemyU_C->texturePath.c_str());
        pCatEnemyM2D_C->texture = engine::util::Texture::load(pCatEnemyU_C->texturePath.c_str());
    });
    CombatController::enemyDead.addListener([&]
    {
        auto eInfM2D_C = &componentManager.getComponent<Model2D>(eInfID_E);
        auto eArcM2D_C = &componentManager.getComponent<Model2D>(eArcID_E);
        auto eCatM2D_C = &componentManager.getComponent<Model2D>(eCatID_E);

        glDeleteTextures(1, &eInfM2D_C->texture);
        glDeleteTextures(1, &eArcM2D_C->texture);
        glDeleteTextures(1, &eCatM2D_C->texture);
    });
}

struct MouseInput {
    bool pressed = false;
    bool clicked = false;

    void update(GLFWwindow* window, int GLFW_BUTTON) {
        bool current = glfwGetMouseButton(window, GLFW_BUTTON) == GLFW_PRESS;
        clicked = (current && !pressed);
        pressed = current;
    }
};

MouseInput left;
MouseInput right;

void Game::update(GLFWwindow* window) {
    auto selectionSystem = &getSystem<SelectionSystem>();
    auto combatController = &getSystem<CombatController>();
    auto pCombatSelection = CombatSelection::GetInstance();

    left.update(window, GLFW_MOUSE_BUTTON_LEFT);
    if (left.clicked)
    {
        Unit* pSelection = selectionSystem->select<Unit, UnitState>(*this, 0.1f);
        if (pCombatSelection->SetSelection(pSelection)) {
            auto pSoundSystem = engine::SoundSystem::GetInstance();
            pSoundSystem->PlaySound(engine::UI_BUTTON_PRESS);
            pCombatSelection->HandleEdgeCases(*this);
        }
    }

    right.update(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (right.clicked)
    {
        pCombatSelection->ResetSelections();
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    combatController->handleTurn();
    elapsedTime += deltaTime;
}

void Game::draw() {
}
