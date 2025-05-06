//
// Created by julia on 25/02/2025.
//

#include "GuiContainer.h"

#include <iostream>
#include <stb_image.h>
#include "GuiCombat.h"

using namespace gui;

GuiContainer::GuiContainer(GLFWwindow* window):window(window)
{
    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas);
        struct nk_font *FantasyRPG1 = nk_font_atlas_add_from_file(atlas, "assets/textures/gui/FantasyRPG1.ttf", 20, 0);
        nk_glfw3_font_stash_end(&glfw);
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &FantasyRPG1->handle);}
}

GuiContainer::~GuiContainer()
{
    nk_glfw3_shutdown(&glfw);
}

void GuiContainer::init()
{
   loadTextureAtlas("assets/textures/gui/ui_atlas_48x48.png");
   GuiCombat combatMenu(ctx, textureAtlasID);
}

void GuiContainer::draw()
{
}

void GuiContainer::update()
{
    nk_glfw3_new_frame(&glfw);
}

void GuiContainer::loadTextureAtlas(const char* texturePath)
{
    int width, height, channels;
    unsigned char* image = stbi_load(texturePath, &width, &height, &channels, 4); // Load as RGBA

    if (!image) {
        std::cerr << "Failed to load texture atlas: " << texturePath << "\n";
        return;
    }

    glGenTextures(1, &textureAtlasID);
    glBindTexture(GL_TEXTURE_2D, textureAtlasID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
}
