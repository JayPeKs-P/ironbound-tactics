//
// Created by julia on 26/11/2024.
//

#include "guiElement.h"

using namespace gl3;

guiElement::guiElement(PngTexture pngTexture):
pngTexture(std::move(pngTexture))

{
    textureID = pngTexture.getTextureID();
    textureHeight = pngTexture.getPngHeight();
    textureWidth = pngTexture.getPngWidth();
}

guiElement::~guiElement()
{
}

