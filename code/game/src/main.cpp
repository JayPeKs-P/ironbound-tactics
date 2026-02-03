#include <iostream>

#include "Game.h"

int main()
{
    try
    {
        gl3::Game ironBoundTactics(1280, 720, "Ironbound Tactics");
        ironBoundTactics.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unhandled exception " << e.what() << std::endl;
    }
}
