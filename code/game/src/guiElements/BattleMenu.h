//
// Created by julia on 27/11/2024.
//

#pragma once
#include "guiElement.h"

namespace gl3 {

class BattleMenu: public guiElement {
public:
    BattleMenu(Game* game);
    void renderBattleMenu();

};

} // gl3