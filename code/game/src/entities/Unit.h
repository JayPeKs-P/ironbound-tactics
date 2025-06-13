//
// Created by Julian Kalb on 12/12/2024.
//

#pragma once
#include <random>
namespace gl3 {

class Unit{
public:
    Unit(float hp, float def, float atk, int acc, int critc);
    ~Unit() = default;
    void move();
    float attack();
    void use();
    float takeDamage(float damage);

    [[nodiscard]] bool isReady() const
    {
        return ready;
    }

    [[nodiscard]] float getLifePoints() const
    {
        return lifePoints;
    }

    void setReady(bool state)
    {
        ready = state;
    }

protected:
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, 99};
    float lifePoints;
    float armorValue;
    float attackValue;
    int accuracy;
    int critChance;
    float critMultiplier = 2.0f;
    bool ready = true;

};

} // gl3