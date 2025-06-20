#include <stdexcept>
#include "engine/Game.h"

using namespace gl3::engine;
Game::Game(int width, int height, const std::string& title):
context(width, height, title),
componentManager(*this),
entityManager(componentManager,*this)
{

}

Game::~Game()
{
    context.~Context();
}

glm::mat4 Game::calculateMvpMatrix(glm::vec3 position, float zRotationDegrees, glm::vec3 scale)
{
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(zRotationDegrees), glm::vec3(0, 0, 1));

    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 90.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::perspective(glm::radians(2.0f),1000.0f/600.0f,0.1f,100.0f);

    glm::mat4 mvpMatrix = projection * view * model;
    return mvpMatrix;
}

void Game::run()
{
    onStartup.invoke(*this);
    start();
    onAfterStartup.invoke(*this);
    context.run([&](context::Context &gl3CTX)
    {
        onBeforeUpdate.invoke(*this);
        update(getWindow());
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