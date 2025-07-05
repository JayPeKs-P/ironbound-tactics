//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"

#include "../components/TagComponent.h"


using namespace gl3;

CombatController::CombatController(engine::Game &game):
    System(game)
{
}

CombatController::~CombatController()
{
}



void CombatController::init(engine::Game &game)
{
    auto &infContainer = game.componentManager.getContainer<UnitContainer<Infantry>>();
    auto &arcContainer = game.componentManager.getContainer<UnitContainer<Archer>>();
    auto &catContainer = game.componentManager.getContainer<UnitContainer<Catapult>>();
    for (auto &[owner, _] : infContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pInfContainer = &game.componentManager.getComponent<UnitContainer<Infantry>>(owner);
            pInfContainer->add(10);
        }
    }
    for (auto &[owner, _] : arcContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pArcContainer = &game.componentManager.getComponent<UnitContainer<Archer>>(owner);
            pArcContainer->add(15);
        }
    }
    for (auto &[owner, _] : catContainer)
    {
        auto &tagComponent = game.componentManager.getComponent<TagComponent>(owner).value;
        if (tagComponent == Tag::PLAYER)
        {
            pCatContainer = &game.componentManager.getComponent<UnitContainer<Catapult>>(owner);
            pCatContainer->add(5);
        }
    }
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