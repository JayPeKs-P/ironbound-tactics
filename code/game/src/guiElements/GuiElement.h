#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../rendering/PngTexture.h"

namespace gl3 {
class Game;
class GuiElement {
    public:
    GuiElement(PngTexture pngTexture,
        int tileWidth,
        int tileHeight);
    virtual ~GuiElement();


    private:
    PngTexture pngTexture;
    void initializeGuiElement();
    void drawTile(const std::pair<int, int>& tile, int tileID);

    protected:
    GLuint textureAtlasID;
    int textureAtlasWidth;
    int textureAtlasHeight;
    int tileWidth;
    int tileHeight;

    ImVec2 getTileUV(int column, int row);
    void drawBackground(const std::vector<std::pair<int, int>>& tilesToRender,
        int backgroundWidth, int backgroundHeight);


};

} // gl3