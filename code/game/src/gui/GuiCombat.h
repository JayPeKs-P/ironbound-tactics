//
// Created by julia on 25/02/2025.
//

#pragma once
#include "nuklear.h"

namespace gui {

class GuiCombat {
    public:
    GuiCombat(struct nk_context *ctx, nk_uint textureID);
    ~GuiCombat() = default;

    private:
    nk_uint textureID;

    struct nk_image GuiCombat::getTileImage(
    int tileX,
    int tileY,
    int tileWidth,
    int tileHeight,
    int atlasWidth,
    int atlasHeight);

    // struct nk_image atlas;

    void setStyleWindow(nk_style* style);
    void setStyleButton(nk_style* style);
    void setStyleOptionLabel(nk_style* style);
    void setStyleProgressBar(nk_style* style);
    void setStyleSlider(nk_style* style);
    void setStyleText(nk_style* style);

    nk_context* ctx;


};

} // gl3