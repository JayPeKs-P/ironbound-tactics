//
// Created by julia on 25/02/2025.
//

#include "GuiContainer.h"

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
}

void GuiContainer::draw()
{
}

void GuiContainer::update()
{
    nk_glfw3_new_frame(&glfw);
}
