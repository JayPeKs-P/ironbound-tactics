//
// Created by Julian Kalb on 20/07/2025.
//
#pragma once
#include <cassert>
#include <iostream>

#ifdef DEBUG_MODE
    #define DEBUG_LOG(x) std::cout << x << std::endl
#else
    #define DEBUG_LOG(x)
#endif


inline std::string unitCategory_to_string (UnitCategory category)
{
#pragma warning(push)
#pragma warning(default : 4091)
    switch (category)
    {
    case UnitCategory::INFANTRY:
        return "Infantry";
    case UnitCategory::ARCHER:
        return "Archer";
    case UnitCategory::CATAPULT:
        return "Catapult";
    case UnitCategory::ASSAULT_COVER:
        return "Assault Cover";
    case UnitCategory::SIEGE:
        return "Siege";
    }
#pragma warning(pop)
    assert(false);
    return "fail";
}
