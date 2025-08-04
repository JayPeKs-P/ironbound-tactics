#include "engine/Game.h"
#include "engine/ecs/System.h"
#include "Transform.h"

using gl3::engine::ecs::System;
using gl3::engine::Game;
using gl3::engine::sceneGraph::Transform;

namespace gl3::engine::sceneGraph {

    /// @brief The SceneGraphUpdater goes through a list of all Transforms and calculates their model-matrix.
    class SceneGraphUpdater : public System {
    public:
        /// Adds updateTransforms() to @ref engine::Game::onUpdate as listener.
        ///
        /// @param game Reference to the core instance of @ref engine::Game
        explicit SceneGraphUpdater(Game &game) : System(game) {
            game.onUpdate.addListener([&](Game &) {
                updateTransforms(game);
            });
        }

        /// @brief Calculate model-matrix for each Transform currently registered in @ref engine::Game::componentManager.
        ///
        /// @param game Reference to the core instance of @ref engine::Game
        static void updateTransforms(Game &game) {
            game.componentManager.forEachComponent<Transform>([&](Transform &transform) {
                transform.modelMatrix = calculateModelMatrix(transform);
            });
        }

    private:
        static glm::mat4 calculateModelMatrix(Transform &transform) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, transform.localPosition);
            model = glm::scale(model, transform.localScale);
            model = glm::rotate(model, glm::radians(transform.localZRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            return model;
        }
    };
}