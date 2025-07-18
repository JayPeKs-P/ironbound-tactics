#include <stdexcept>
#include "engine/Game.h"

#include "engine/sceneGraph/SceneGraphPruner.h"
#include "engine/sceneGraph/SceneGraphUpdater.h"

using namespace gl3::engine;
Game::Game(int width, int height, const std::string& title):
context(width, height, title),
componentManager(*this),
entityManager(componentManager,*this)
{
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

void Game::run()
{
    sceneGraph::SceneGraphUpdater sceneGraphUpdater(*this);
    sceneGraph::SceneGraphPruner sceneGraphPruner(*this);
    onStartup.invoke(*this);
    start();
    onAfterStartup.invoke(*this);
    sceneGraphUpdater.updateTransforms(*this);
    context.run([&](context::Context &gl3CTX)
    {
        onBeforeUpdate.invoke(*this);
        update(getWindow());
        onUpdate.invoke(*this);
        draw();
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