//
// Created by julia on 29/10/2024.
//

#include "Enemy.h"


#include "../Game.h"


using namespace gl3;

Enemy::Enemy(glm::vec3 position, float zRotation, float size):
Entity(Shader("shaders/shaded/vertexShader.vert",
    "shaders/shaded/fragmentShader.frag"),
    Mesh("gltf/enemy.glb"),
    position,
    zRotation,
    {size,size,size},
    {0.3f, 0.0f,0.0f,1})
{
}
float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}
void Enemy::update(Game* game, float deltaTime)
{
    std::time_t elapsedTime = std::time(nullptr);
    const auto shipPosition = game->getShip()->getPosition();
    float distanceToShip = glm::distance(position, shipPosition);
    float delta_y = shipPosition.y - this->position.y;
    float delta_x = shipPosition.x - this->position.x;
    float theta_radians = atan2(delta_y, delta_x);

    zRotation = glm::degrees(theta_radians);

    if(distanceToShip >= 0.5) {
        position.x = lerp(position.x, shipPosition.x, deltaTime * speed);
        position.y = lerp(position.y, shipPosition.y, deltaTime * speed);
    }
    countdownUntilNextShot -= deltaTime;
    if(countdownUntilNextShot <= 0 && elapsedTime % 5 == 0)
    {
        auto missile = std::make_unique<Missile>(game);
        missile->setPosition(this->getPosition());
        missile->setZRotation(this->getZRotation()-90);
        missile->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
        missiles.push_back(std::move(missile));
        countdownUntilNextShot = timeBetweenShots;
    }
    for(auto& missile : missiles)
    {
        missile->update(game, deltaTime);
    }
    if(missiles.size() >= 100)
    {
        missiles.erase(missiles.begin(), missiles.end());
    }
}

void Enemy::draw(Game* game)
{
    Entity::draw(game);
    for(auto& missile : missiles)
    {
        missile->draw(game);
    }
}
