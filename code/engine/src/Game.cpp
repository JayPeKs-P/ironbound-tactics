#include "engine/Log.h"
#include "engine/Game.h"

#include "engine/sceneGraph/SceneGraphPruner.h"
#include "engine/sceneGraph/SceneGraphUpdater.h"
#include "engine/rendering/RenderSystem.h"
#include "engine/util/Assets.h"
#include <soloud.h>
#include <soloud_wav.h>
#include "engine/SoundSystem.h"

#include "engine/Texture.h"

using namespace gl3::engine;
Game::Game(int width, int height, const std::string& title):
context(width, height, title),
componentManager(*this),
entityManager(componentManager,*this)
{
    Log::init();
    origin = &entityManager.createEntity().addComponent<sceneGraph::Transform>();
    // m_pAudioPlayer = std::make_unique<SoLoud::Soloud>();
    // m_pAudioPlayer->init();
    // m_pAudioPlayer->setGlobalVolume(0.3f);
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

// void Game::PlaySound(const std::string& sFileName) {
//     if (m_ListHandles.contains(sFileName)) {
//         auto& sound = m_ListHandles[sFileName];
//         if (m_pAudioPlayer->isValidVoiceHandle(sound)) {
//             return;
//         }
//     }
//     auto handle = m_pAudioPlayer->play(*m_ListSound[sFileName]);
//     m_ListHandles.insert({sFileName, handle});
// }
//
// void Game::PlayMusic(const std::string& sFileName) {
//     auto& music = m_ListMusic[sFileName];
//     if (music == m_pCurrentMusic) return;
//     if (m_iCurrentMusic > -1) m_pAudioPlayer->stop(m_iCurrentMusic);
//     m_iCurrentMusic = m_pAudioPlayer->play(*m_ListMusic[sFileName]);
//     m_pCurrentMusic = m_ListMusic[sFileName];
// }
//
// void Game::RegisterSound(const std::string& sFileName) {
//     auto sound = std::make_unique<SoLoud::Wav>();
//     sound->load(resolveAssetPath("audio/" + sFileName).string().c_str());
//     sound->setSingleInstance(true);
//     m_ListSound[sFileName] = std::move(sound);
// }
//
// void Game::RegisterMusic(const std::string& sFileName) {
//     auto music = std::make_unique<SoLoud::Wav>();
//     music->load(resolveAssetPath("audio/" + sFileName).string().c_str());
//     music->setSingleInstance(true);
//     music->setLooping(true);
//     m_ListMusic[sFileName] = std::move(music);
// }

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