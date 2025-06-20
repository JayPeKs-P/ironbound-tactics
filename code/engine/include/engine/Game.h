#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Context.h"
#include "engine/Events.h"
#include "engine/ecs/ComponentManager.h"
#include "engine/ecs/EntityManager.h"

namespace gl3::engine {
    class Game{
    public:
        using event_t = events::Event<Game, Game&>;
        // List of core events
        event_t onStartup;
        event_t onAfterStartup;
        event_t onBeforeUpdate;
        event_t onAfterUpdate;
        event_t onBeforeShutdown;
        event_t onShutdown;

        engine::ecs::ComponentManager componentManager;
        engine::ecs::EntityManager entityManager;

        virtual void run();
        static glm::mat4 calculateMvpMatrix(glm::vec3 position, float zRotationDegrees, glm::vec3 scale);
        GLFWwindow *getWindow() {return context.getWindow();}
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