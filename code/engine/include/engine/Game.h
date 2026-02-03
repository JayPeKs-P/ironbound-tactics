#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>

#include "json.hpp"
#include "engine/Context.h"
#include "engine/Events.h"
#include "engine/ecs/ComponentManager.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/System.h"

#include "engine/sceneGraph/Transform.h"

#include "engine/fsm/StateMachine.h"

#include "logic/ActionRegistry.h"


// namespace SoLoud {
//     class Wav;
//     class Soloud;
// }

namespace gl3::engine {
    /// @brief Core game class, that manages the main structures the main loop and manages core systems and ECS state.
    ///
    /// This class initializes the core components (context, ECS, rendering, events)
    /// and defines the core structure of a games lifecycle and update loop.
    class Game{
    public:
        using event_t = events::Event<Game, Game&>;
        using SystemContainer = std::unordered_map<std::type_index, std::unique_ptr<ecs::System>>;

        template<typename S, typename ...Args>
        [[nodiscard]] S &addSystem(Args&& ...args) {
            auto system = std::make_unique<S>(*this, std::forward<Args>(args)...);
            S& systemRef = *system;
            systems[std::type_index(typeid(S))] = std::move(system);
            return systemRef;
        }
        template<typename S>
        S &getSystem() {
            auto it = systems.find(std::type_index(typeid(S)));
            if(it == systems.end())
            {
                throw std::runtime_error("SystemManager: System not found" + std::string(typeid(S).name()));
            }
            return *static_cast<S*>(it->second.get());
        }
        /// Runs the game. Invokes all core events and alls start() function.
        ///
        /// @brief Starts the main game loop.
        ///
        /// This function initializes the rendering system, the updater and pruner of the Scene Graph.
        /// It then calls the start() function, invokes all @a core @a events and enters the core loop via
        /// 'context.run()', invoking update() and draw() and the corresponding core events.
        ///
        /// Lifecycle events triggered (in order):
        /// - onStartup
        /// - onAfterStartup
        /// - onBeforeUpdate → update() → onUpdate → draw() → renderer.draw() → updateDeltaTime → onAfterUpdate
        /// - onBeforeShutdown
        /// - onShutdown
        virtual void run();

        /// @brief Calculates the model-view-projection matrix.
        ///
        /// @param model Local model transformation matrix.
        /// @return Computed MVP matrix.
        static glm::mat4 calculateMvpMatrix(glm::mat4 model);

        /// @brief Returns the GLFW window instance.
        ///
        /// @return Pointer to GLFWwindow.
        GLFWwindow *getWindow() {return context.getWindow();}
        void ToggleFullScreen();

        ///@brief Returns the current value of the delta-time variable.
        ///
        ///@return Value of delta-time.
        float getDeltaTime() {return deltaTime;}

        unsigned int AddTextureToRegistry(const char* pPath, const char* pTextureName = nullptr);
        unsigned int GetTextureFromRegistry(const char* pKey);


        /// @defgroup CoreEvents Core Game Events
        /// @{
        /// @brief Core event, triggered before the start() call.
        /// @note Registered Listeners: see grouping.
        /// @ingroup CoreEvents
        event_t onStartup;
        /// Core event, triggered right after the start() call and before entering the update cycle.
        event_t onAfterStartup;

        /// Core event, triggered before every update cycle.
        event_t onBeforeUpdate;
        /// Core event, triggered after the update() call in the loop.
        event_t onUpdate;
        /// Core event, triggered at the end of the update cycle.
        event_t onAfterUpdate;

        /// Core event, triggered right after leaving the update cycle.
        event_t onBeforeShutdown;
        /// Core event, triggered at shutdown.
        event_t onShutdown;
        /// @}

        ComponentManager componentManager;
        ecs::EntityManager entityManager;
        sceneGraph::Transform* origin = nullptr;

        fsm::StateMachine SceneManagerFSM;

        engine::combat::ActionRegistry actionRegister;
        float elapsedTime = 0.0f;
        [[nodiscard]] float GetSpeedUpValue() const {return m_fSpeedupValue;}
        void SetSpeedUpValue(float value) {m_fSpeedupValue = value;}
        const char* GetConfigPath() const {return m_pConfigPath;}
        nlohmann::basic_json<> GetConfigEntry(const char* pKey) const;
        void SetConfigEntry(const char* pKey, const char* pValue);
    protected:
        /// @brief Constructs the game with window dimensions and title.
        ///
        /// Initializes Transform origin.
        /// @param width Width of the window.
        /// @param height Height of the window.
        /// @param title Title of the game.
        /// All parameters are used to initialize context.
        Game(int width, int height, const std::string& title);
        virtual ~Game();

        /// @brief Called once at the beginning of the game.
        virtual void start(){};

        /// @brief Called every frame to update game logic.
        /// @param window Pointer to a GLFWwindow.
        virtual void update(GLFWwindow *window){};

        /// @brief Called every frame to render graphics.
        virtual void draw(){};

        /// @brief Updates the delta time based on glfwGetTime().
        virtual void updateDeltaTime();

        /// @brief Time in seconds since the last frame.
        float deltaTime = 1.0f / 60;
    private:
        const char* m_pConfigPath = "assets/config.json";
        void LoadConfig();
        float m_fSpeedupValue = 1.0f;
        SystemContainer systems;
        context::Context context;
        context::WindowedData m_WindowedData;
        std::unordered_map<std::string, unsigned int> m_TextureRegistry = {};


        /// @brief Timestamp of the last frame for delta calculation.
        float lastFrameTime = 1.0f / 60;

      };
}
