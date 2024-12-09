// //
// // Created by julia on 29/10/2024.
// //
//
// #pragma once
// #include "Entity.h"
// #include "Missile.h"
//
// #include <soloud.h>
// #include <soloud_wav.h>
//
// namespace gl3 {
//
// class Ship: public Entity {
//     public:
//         explicit Ship(glm::vec3 position = glm::vec3(0.0f, 0.0f,0.0f),
//             float zRotation = 0.0f,
//             glm::vec3 scale= glm::vec3(0.25f, 0.25f, 0.25f));
//         void update(Game* game, float deltaTime) override;
//         void draw(Game* game) override;
//
//     private:
//         std::vector<std::unique_ptr<Missile>> missiles;
//         SoLoud::Soloud audio;
//         SoLoud::Wav firingSound;
//         const float timeBetweenShots = 0.1f;
//         float countdownUntilNextShot = timeBetweenShots;
//         float translationSpeed = 1.0f;
//         float rotationSpeed = 120.0f;
//
//         // std::vector<unsigned int> indices = {
//         //     //body of ship
//         //     0, 1, 2,    //triangle 1: v1, v2, v3
//         //     //wing left
//         //     3, 1, 4,    //triangle 2: v4, v2, v5
//         //     //wing right
//         //     5, 2, 6,    //triangle 3: v6, v3, v7
//         //     //jet engine
//         //     7, 3, 5    //triangle : v8, v4, v6
//         // };
//
//
// };
//
// } // gl3