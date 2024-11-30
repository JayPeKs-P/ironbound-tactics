#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../rendering/PngTexture.h"

namespace gl3 {
class Game;
class guiElement {
    public:
    guiElement(PngTexture pngTexture);
    virtual ~guiElement();

    private:
    PngTexture pngTexture;
    //virtual void render();

    protected:
    GLuint textureID;
    int textureWidth;
    int textureHeight;


};

} // gl3