// //
// // Created by julia on 29/10/2024.
// //
//
// #pragma once
// #include "Entity.h"
// #include "Ship.h"
//
// namespace gl3 {
//
// class Enemy: public Entity{
// public:
//     explicit Enemy(glm::vec3 position = glm::vec3(2.0f, -1.0f,0.0f),
//         float zRotation = 0.0f,
//         float size = 0.25f);
//     void update(Game* game, float deltaTime) override;
//     void draw(Game* game) override;
//
//
// private:
//     std::vector<std::unique_ptr<Missile>> missiles;
//     const float timeBetweenShots = 0.1f;
//     float countdownUntilNextShot = timeBetweenShots;
//     float speed = 0.5f;
//
//     // std::vector<float> vertices = {
//     //
//     //     0.8f, 0.0f, 0.0f,   // v1
//     //     0.0f, 0.2f, 0.0f,   // v2
//     //     0.0f, -0.2f, 0.0f,   // v3
//     //     0.0f, 0.05f, 0.0f,   // v4
//     //     -0.15f, 0.2f, 0.0f,   // v5
//     //     0.0f, -0.05f, 0.0f,   // v6
//     //     -0.15f, -0.2f, 0.0f,   // v7
//     //     -0.25f, 0.0f, 0.0f,   // v8
//     // };
//     // std::vector<unsigned int> indices = {
//     //     //body of ship
//     //     0, 1, 2,    //triangle 1: v1, v2, v3
//     //     //wing left
//     //     3, 1, 4,    //triangle 2: v4, v2, v5
//     //     //wing right
//     //     5, 2, 6,    //triangle 3: v6, v3, v7
//     //     //jet engine
//     //     7, 3, 5    //triangle : v8, v4, v6
//     // };
// };
//
// } // gl3