//
// Created by julia on 27/11/2024.
//

#pragma once
#include "guiElement.h"

namespace gl3 {

class BattleMenu: public guiElement {
public:
    BattleMenu(const ImVec2& size);
    void renderBattleMenu();
private:
    const std::vector<std::pair<int, int>> tilesToRender = {
        {15, 0},
        {16, 0},
        {17, 0},
        {15, 1},
        {16, 1},
        {17, 1},
        {15, 2},
        {16, 2},
        {17, 2}
    };
    ImVec2 size;
    float enemyLifePoints = 1.0f;
    float playerLifePoints = 1.0f;

};

} // gl3