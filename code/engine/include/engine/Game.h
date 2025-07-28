#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Context.h"
#include "engine/Events.h"
#include "engine/ecs/ComponentManager.h"
#include "engine/ecs/EntityManager.h"
#include "engine/sceneGraph/Transform.h"

#include "logic/ActionRegistry.h"


namespace gl3::engine {
    class Game{
    public:
        using event_t = events::Event<Game, Game&>;

        virtual void run();
        static glm::mat4 calculateMvpMatrix(glm::mat4 model);
        GLFWwindow *getWindow() {return context.getWindow();}
        float getDeltaTime() {return deltaTime;}

        // List of core events
        event_t onStartup;
        event_t onAfterStartup;
        event_t onBeforeUpdate;
        event_t onUpdate;
        event_t onAfterUpdate;
        event_t onBeforeShutdown;
        event_t onShutdown;

        ComponentManager componentManager;
        ecs::EntityManager entityManager;
        sceneGraph::Transform* origin = nullptr;

        engine::combat::ActionRegistry actionRegister;
        float elapsedTime = 0.0f;
    protected:
        Game(int width, int height, const std::string& title);
        virtual ~Game();

        virtual void start(){};
        virtual void update(GLFWwindow *window){};
        virtual void draw(){};
        virtual void updateDeltaTime();

        float deltaTime = 1.0f / 60;
    private:
        glm::mat4 mvpMatrix{};
        context::Context context;
        float lastFrameTime = 1.0f / 60;

      };
}
