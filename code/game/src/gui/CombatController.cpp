//
// Created by Julian Kalb on 25/02/2025.
//

#include "CombatController.h"

#include <iostream>
#include <stb_image.h>
#include "GuiCombat.h"
#include "../entities/unitTypes/Archer.h"
#include "../entities/unitTypes/Infantry.h"
#include "../entities/objectTypes/Catapult.h"

using namespace gl3;

CombatController::CombatController(engine::Game &game):
    System(game),
window(game.getWindow())
{
    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas);
        struct nk_font *FantasyRPG1 = nk_font_atlas_add_from_file(atlas, "assets/textures/gui/FantasyRPG1.ttf", 20, 0);
        nk_glfw3_font_stash_end(&glfw);
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &FantasyRPG1->handle);}

}

CombatController::~CombatController()
{
    nk_glfw3_shutdown(&glfw);
}



void CombatController::init()
{
   loadTextureAtlas("assets/textures/gui/ui_atlas_48x48.png");
   guiCombat = new gui::GuiCombat(ctx, textureAtlasID);


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
    // handleTurn(true);
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

void CombatController::handleTurn(bool newRound)
{
    // float pHP = 0;
    // for (auto& unit: playerUnits)
    // {
    //     pHP += unit->getLifePoints();
    // }
    // std::cout<<"Player HP: "<<pHP<<std::endl;
    // float eHP = 0;
    // for (auto& unit: enemyUnits)
    // {
    //     eHP += unit->getLifePoints();
    // }
    // std::cout<<"Enemy HP: "<<eHP<<std::endl;
    // std::cout<<"Attack with Inf? [y/n]"<<std::endl;
    // char input;
    // std::cin>>input;
    // if (input=='y')
    // {
    //     std::cout<<"Target? [a/i]"<<std::endl;
    //     std::cin>>input;
    //     if (input=='a')
    //     {
    //         enemyUnits[1]->takeDamage(playerUnits[0]->attack());
    //     }
    //     if (input=='i')
    //     {
    //         enemyUnits[0]->takeDamage(playerUnits[0]->attack());
    //     }
    // }
}