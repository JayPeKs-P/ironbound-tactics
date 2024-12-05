//
// Created by julia on 26/11/2024.
//

#include "guiElement.h"


using namespace gl3;

guiElement::guiElement(PngTexture pngTexture, int tileWidth, int tileHeight):
pngTexture(std::move(pngTexture)),
tileWidth(tileWidth),
tileHeight(tileHeight)

{
    initializeGuiElement();
}

guiElement::~guiElement()
{
}

void guiElement::initializeGuiElement()
{
    textureAtlasID = pngTexture.getTextureID();
    textureAtlasHeight = pngTexture.getPngHeight();
    textureAtlasWidth = pngTexture.getPngWidth();
}

void guiElement::drawTile(const std::pair<int, int>& tile, int tileID)
{
    const ImVec2 uv0 = getTileUV(tile.first, tile.second);
    const ImVec2 uv1 = getTileUV(tile.first + 1, tile.second + 1);
    ImGui::PushID(tileID);
    ImGui::Image(textureAtlasID, ImVec2(48, 48), uv0, uv1);
    ImGui::PopID();
}

ImVec2 guiElement::getTileUV(int column, int row)
{
    ImVec2 uv;
    uv.x = (float)(column * tileWidth) / textureAtlasWidth;         // Top-left U
    uv.y = (float)(row * tileHeight) / textureAtlasHeight;       // Top-left V


    return uv;
}

void guiElement::drawBackground(const std::vector<std::pair<int, int>>& tilesToRender,
    int backgroundWidth,
    int backgroundHeight)
{
    // Calculate number of tiles needed
    int tilesX = backgroundWidth / tileWidth;
    int tilesY = backgroundHeight / tileHeight;

    for (int rangeY = 0; rangeY < tilesY; rangeY++) {
        for (int rangeX = 0; rangeX < tilesX; rangeX++) {
            // Determine tile type based on position
            if (rangeX == 0 && rangeY == 0)
                {
                // Top-left corner
                drawTile(tilesToRender[0], rangeX + rangeY);
                ImGui::SameLine(0.0f, 0.0f);
            } else if (rangeX == tilesX - 1 && rangeY == 0)
                {
                // Top-right corner
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
                drawTile(tilesToRender[2], rangeX + rangeY);
                ImGui::PopStyleVar();
            } else if (rangeX == 0 && rangeY == tilesY - 1)
                {
                // Bottom-left corner
                drawTile(tilesToRender[6], rangeX + rangeY);
                ImGui::SameLine(0.0f, 0.0f);
            } else if (rangeX == tilesX - 1 && rangeY == tilesY - 1)
                {
                // Bottom-right corner
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
                drawTile(tilesToRender[8], rangeX + rangeY);
                ImGui::PopStyleVar();
            } else if (rangeY == 0)
                {
                // Top edge
                drawTile(tilesToRender[1], rangeX + rangeY);
                ImGui::SameLine(0.0f, 0.0f);
            } else if (rangeY == tilesY - 1)
            {
                //Bottom edge
                drawTile(tilesToRender[7], rangeX + rangeY);
                ImGui::SameLine(0.0f, 0.0f);
            } else if (rangeX == 0)
                {
                // Left edge
                drawTile(tilesToRender[3], rangeX + rangeY);
                ImGui::SameLine(0.0f, 0.0f);
            } else if (rangeX == tilesX - 1)
                {
                // Right edge
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
                drawTile(tilesToRender[5], rangeX + rangeY);
                ImGui::PopStyleVar();
            } else
                {
                // Middle tiles
                drawTile(tilesToRender[4], rangeX + rangeY);
                ImGui::SameLine(0.0f, 0.0f);
            }
        }
    }
}

