#include "engine/Log.h"
#include "engine/Game.h"

#include "engine/sceneGraph/SceneGraphPruner.h"
#include "engine/sceneGraph/SceneGraphUpdater.h"
#include "engine/rendering/RenderSystem.h"
#include "engine/util/Assets.h"
#include <soloud.h>
#include <soloud_wav.h>

#include "engine/Texture.h"

using namespace gl3::engine;
Game::Game(int width, int height, const std::string& title):
context(width, height, title),
componentManager(*this),
entityManager(componentManager,*this)
{
    Log::init();
    origin = &entityManager.createEntity().addComponent<sceneGraph::Transform>();
    m_pAudioPlayer = std::make_unique<SoLoud::Soloud>();
    m_pAudioPlayer->init();
    m_pAudioPlayer->setGlobalVolume(0.1f);
    RegisterSound("retro_ui_menu_simple_click_12.wav");
    RegisterSound("retro_ui_menu_simple_click_03.wav");
    RegisterSound("retro_ui_menu_blip_click_20.wav");
    RegisterSound("retro_footstep_movement_05.wav");
    RegisterSound("retro_impact_colorful_01.wav");
    RegisterMusic("Retro Action Game Theme #6 (looped).wav");
    RegisterMusic("Retro Action Game Theme #8 (looped).wav");
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

void Game::PlaySound(const char* pFileName) {
    m_pAudioPlayer->play(*m_ListSound[pFileName]);
}

void Game::PlayMusic(const char* pFileName) {
    auto& music = m_ListMusic[pFileName];
    if (music == m_pCurrentMusic) return;
    if (m_iCurrentMusic > -1) m_pAudioPlayer->stop(m_iCurrentMusic);
    m_iCurrentMusic = m_pAudioPlayer->play(*m_ListMusic[pFileName]);
    m_pCurrentMusic = m_ListMusic[pFileName];
}

void Game::RegisterSound(const char* pFileName) {
    auto sound = std::make_unique<SoLoud::Wav>();
    std::string fileName = pFileName;
    sound->load(resolveAssetPath("audio/" + fileName).string().c_str());
    sound->setSingleInstance(true);
    m_ListSound[fileName] = std::move(sound);
}

void Game::RegisterMusic(const char* pFileName) {
    auto music = std::make_unique<SoLoud::Wav>();
    std::string fileName = pFileName;
    music->load(resolveAssetPath("audio/" + fileName).string().c_str());
    music->setSingleInstance(true);
    music->setLooping(true);
    m_ListMusic[fileName] = std::move(music);
}

void Game::run()
{
    auto sceneGraphUpdater = &addSystem<sceneGraph::SceneGraphUpdater>();
    // sceneGraph::SceneGraphUpdater sceneGraphUpdater(*this);
    auto sceneGraphPruner = &addSystem<sceneGraph::SceneGraphPruner>();
    // sceneGraph::SceneGraphPruner sceneGraphPruner(*this);

    auto renderer = &addSystem<render::RenderSystem>();
    // render::RenderSystem renderer(*this);


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