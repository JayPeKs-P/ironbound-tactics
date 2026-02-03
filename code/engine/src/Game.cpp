#include "engine/Log.h"
#include "engine/Game.h"

#include <fstream>

#include "engine/sceneGraph/SceneGraphPruner.h"
#include "engine/sceneGraph/SceneGraphUpdater.h"
#include "engine/rendering/RenderSystem.h"
#include "engine/util/Assets.h"
#include <soloud.h>
#include <soloud_wav.h>

#include "json.hpp"
#include "engine/SoundSystem.h"

#include "engine/Texture.h"

using namespace gl3::engine;

nlohmann::basic_json<> Game::GetConfigEntry(const char* pKey) const {
    using json = nlohmann::json;
    json config;
    std::ifstream in(m_pConfigPath);
    in >> config;
    return config[pKey];
}

void Game::SetConfigEntry(const char* pKey, const char* pValue) {
    using json = nlohmann::json;
    json config;
    std::ifstream in(m_pConfigPath);
    in >> config;

    config[pKey] = pValue;
    std::ofstream out(m_pConfigPath);
    out << config.dump(4);
}


Game::Game(int width, int height, const std::string& title):
context(width, height, title),
componentManager(*this),
entityManager(componentManager,*this)
{
    Log::init();
    origin = &entityManager.createEntity().addComponent<sceneGraph::Transform>();
}

Game::~Game()
{
    context.~Context();
}

glm::mat4 Game::calculateMvpMatrix(glm::mat4 model)
{
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 90.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::perspective(glm::radians(2.0f),1000.0f/600.0f,0.1f,100.0f);

    glm::mat4 mvpMatrix = projection * view * model;
    return mvpMatrix;
}

void Game::ToggleFullScreen() {
    GLFWmonitor* pCurrentMonitor = glfwGetWindowMonitor(context.getWindow());
    if (!pCurrentMonitor) {
        glfwGetWindowPos(context.getWindow(),
                        &m_WindowedData.iWindowedX, &m_WindowedData.iWindowedY);
        glfwGetWindowSize(context.getWindow(),
                        &m_WindowedData.iWindowedWidth, &m_WindowedData.iWindowedHeight);
        GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* pVidMode = glfwGetVideoMode(pMonitor);

        glfwSetWindowMonitor(
            context.getWindow(),
            pMonitor,
            0, 0,
            pVidMode->width,
            pVidMode->height,
            pVidMode->refreshRate);
    }
    else {
        glfwSetWindowMonitor(
            context.getWindow(),
            nullptr,
            m_WindowedData.iWindowedX,
            m_WindowedData.iWindowedY,
            m_WindowedData.iWindowedWidth,
            m_WindowedData.iWindowedHeight,
            0);
    }
}

unsigned int Game::AddTextureToRegistry(const char* pPath, const char* pTextureName) {
    unsigned int texID = engine::util::Texture::load(pPath);
    if (pTextureName)
    {
        m_TextureRegistry[pTextureName] = texID;
        return texID;
    }
    m_TextureRegistry[pPath] = texID;
}

unsigned int Game::GetTextureFromRegistry(const char* pKey) {
    if (m_TextureRegistry.contains(pKey))
    {
        return m_TextureRegistry[pKey];
    }
    return -1;
}

void Game::run()
{
    LoadConfig();
    auto sceneGraphUpdater = &addSystem<sceneGraph::SceneGraphUpdater>();
    auto sceneGraphPruner = &addSystem<sceneGraph::SceneGraphPruner>();
    auto renderer = &addSystem<render::RenderSystem>();


    onStartup.invoke(*this);
    start();
    onAfterStartup.invoke(*this);
    sceneGraphUpdater->updateTransforms(*this);
    context.run([&](context::Context &gl3CTX)
    {
        onBeforeUpdate.invoke(*this);
        update(getWindow());
        onUpdate.invoke(*this);
        draw();
        renderer->draw(*this);
        updateDeltaTime();
        onAfterUpdate.invoke(*this);
    });
    onBeforeShutdown.invoke(*this);
    onShutdown.invoke(*this);
}

void Game::updateDeltaTime()
{
    float frameTime = glfwGetTime();
    deltaTime = frameTime - lastFrameTime;
    lastFrameTime = frameTime;
}

void Game::LoadConfig() {
    using json = nlohmann::json;
    json config;

    std::ifstream in(m_pConfigPath);
    if (in.good()) {
        try {
            in >> config;
        }
        catch (...) {
            config = {
                {"highscore", 0},
                {"volume", 0.3f},
                {"InfantryTexture", "NONE"},
                {"InfantryTexType", "NONE"},
                {"ArcherTexture", "NONE"},
                {"ArcherTexType", "NONE"},
                {"CatapultTexture", "NONE"},
                {"CatapultTexType", "NONE"}
            };
        }
    }
    else {
        config = {
            {"highscore", 0},
            {"volume", 0.3f},
            {"InfantryTexture", "NONE"},
            {"InfantryTexType", "NONE"},
            {"ArcherTexture", "NONE"},
            {"ArcherTexType", "NONE"},
            {"CatapultTexture", "NONE"},
            {"CatapultTexType", "NONE"}
        };
    }
    config.emplace("highscore", 0);
    config.emplace("volume", 0.3f);
    config.emplace("InfantryTexture", "NONE");
    config.emplace("InfantryTexType", "NONE");
    config.emplace("ArcherTexture", "NONE");
    config.emplace("ArcherTexType", "NONE");
    config.emplace("CatapultTexture", "NONE");
    config.emplace("CatapultTexType", "NONE");

    auto InfantryTexture = config["InfantryTexture"];
    auto ArcherTexture = config["ArcherTexture"];
    auto CatapultTexture = config["CatapultTexture"];
    if (InfantryTexture == "NONE" || ArcherTexture == "NONE" || CatapultTexture == "NONE") {
        InfantryTexture = "assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Soldier_05_Idle.png";
        ArcherTexture = "assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Archer_05_Idle.png";
        CatapultTexture = "assets/textures/entities/Tactical RPG overworld pack 3x/Character sprites/Siege_05_Idle.png";

        config["InfantryTexture"] = InfantryTexture;
        config["InfantryTexType"] =  "BASIC";
        config["ArcherTexture"] = ArcherTexture;
        config["ArcherTexType"] = "BASIC";
        config["CatapultTexture"] = CatapultTexture;
        config["CatapultTexType"] =  "BASIC";
    }

    std::ofstream out(m_pConfigPath);
    out << config.dump(4);
}
