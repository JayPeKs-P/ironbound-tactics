//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"

#include <iostream>
#include <stb_image.h>
#include "GuiCombat.h"
#include "../entities/unitTypes/Archer.h"
#include "../entities/unitTypes/Infantry.h"

using namespace gl3;

CombatController::CombatController(GLFWwindow* window):window(window)
{
    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas);
        struct nk_font *FantasyRPG1 = nk_font_atlas_add_from_file(atlas, "assets/textures/gui/FantasyRPG1.ttf", 20, 0);
        nk_glfw3_font_stash_end(&glfw);
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &FantasyRPG1->handle);}

    auto pInf = std::make_unique<Infantry>();
    auto pArch = std::make_unique<Archer>();
    playerUnits.push_back(std::move(pInf));
    playerUnits.push_back(std::move(pArch));

    auto eInf = std::make_unique<Infantry>();
    auto eArch = std::make_unique<Archer>();
    enemyUnits.push_back(std::move(eInf));
    enemyUnits.push_back(std::move(eArch));
}

CombatController::~CombatController()
{
    nk_glfw3_shutdown(&glfw);
}

void CombatController::init()
{
   loadTextureAtlas("assets/textures/gui/ui_atlas_48x48.png");
    std::cout<<textureAtlasID<<std::endl;
   guiCombat = new gui::GuiCombat(ctx, textureAtlasID);
    std::cout<<textureAtlasID<<std::endl;
}

void CombatController::draw()
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    //maybe create a template for GuiObjects and then draw them, no matter what type they have

    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    guiCombat->drawRender(windowWidth, windowHeight);
}

void CombatController::update()
{
    nk_glfw3_new_frame(&glfw);
}

void CombatController::loadTextureAtlas(const char* texturePath)
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
