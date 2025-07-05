//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"

#include "../entities/unitTypes/Archer.h"
#include "../entities/unitTypes/Infantry.h"
#include "../entities/objectTypes/Catapult.h"

using namespace gl3;

CombatController::CombatController(engine::Game &game):
    System(game)
{
}

CombatController::~CombatController()
{
}



void CombatController::init()
{
}





void CombatController::handleTurn(bool newRound)
{
    // float pHP = 0;
    // for (auto& unit: playerUnits)
    // {
    //     pHP += unit->getLifePoints();
    // }
    // std::cout<<"Player HP: "<<pHP<<std::endl;
    // float eHP = 0;
    // for (auto& unit: enemyUnits)
    // {
    //     eHP += unit->getLifePoints();
    // }
    // std::cout<<"Enemy HP: "<<eHP<<std::endl;
    // std::cout<<"Attack with Inf? [y/n]"<<std::endl;
    // char input;
    // std::cin>>input;
    // if (input=='y')
    // {
    //     std::cout<<"Target? [a/i]"<<std::endl;
    //     std::cin>>input;
    //     if (input=='a')
    //     {
    //         enemyUnits[1]->takeDamage(playerUnits[0]->attack());
    //     }
    //     if (input=='i')
    //     {
    //         enemyUnits[0]->takeDamage(playerUnits[0]->attack());
    //     }
    // }
}