#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace gl3 {
class Game;
class guiElement {
public:
    guiElement(Game* Game);
    virtual ~guiElement();
    /*virtual void initializeGUI(Game *Game);*/

private:


};

} // gl3