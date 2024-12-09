// //
// // Created by julia on 29/10/2024.
// //
//
// #include "Ship.h"
// #include "Missile.h"
//
// #include "../Assets.h"
// #include "../Game.h"
//
// using namespace gl3;
//
// Ship::Ship(glm::vec3 position, float zRotation, glm::vec3 scale):
//     Entity(Shader("shaders/shaded/vertexShader.vert",
//             "shaders/shaded/fragmentShader.frag"),
//             Mesh("gltf/spaceship.glb"),
//         position,
//         zRotation,
//         scale,
//         {0.8f, 0.6f,0.4f,1})
// {
//     audio.init();
//     audio.setGlobalVolume(0.1f);
//     firingSound.load(resolveAssetPath("audio/shot.mp3").string().c_str());
//     firingSound.setSingleInstance(true);
// }
//
// void Ship::update(Game* game, float deltaTime)
// {
//     auto window = game->getWindow();
//     if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//         zRotation += rotationSpeed * deltaTime;
//     }
//
//     if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//         zRotation -= rotationSpeed * deltaTime;
//     }
//
//     if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//         position.y += sin(glm::radians(zRotation)) * translationSpeed * deltaTime;
//         position.x += cos(glm::radians(zRotation)) * translationSpeed * deltaTime;
//     }
//
//     if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//         position.y -= sin(glm::radians(zRotation)) * translationSpeed * deltaTime;
//         position.x -= cos(glm::radians(zRotation)) * translationSpeed * deltaTime;
//     }
//     countdownUntilNextShot -= deltaTime;
//     if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && countdownUntilNextShot <= 0)
//     {
//         auto missile = std::make_unique<Missile>(game);
//         missile->setPosition(this->getPosition());
//         missile->setZRotation(this->getZRotation()-90);
//         missile->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
//         missiles.push_back(std::move(missile));
//         countdownUntilNextShot = timeBetweenShots;
//         audio.play(firingSound);
//     }
//     for(auto& missile : missiles)
//     {
//         missile->update(game, deltaTime);
//     }
//     if(missiles.size() >= 100)
//     {
//         missiles.erase(missiles.begin());
//     }
// }
//
// void Ship::draw(Game* game)
// {
//     Entity::draw(game);
//     for(auto& missile : missiles)
//     {
//         missile->draw(game);
//     }
// }
