//
// Created by Julian Kalb on 05/07/2025.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Events.h"


#define NK_INCLUDE_STANDARD_IO
#include <nuklear_glfw_gl3.h>

#define MAX_VERTEX_BUFFER 1024 *1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "engine/ecs/System.h"
#include "engine/ecs/Gui.h"

enum class GuiScene
{
    MAIN_MENU,
    UNIT_SELECTION,
    COMBAT_MENU,
    LOADING
};
enum Fonts{FANTASY_REGULAR, FANTASY_VERY_LARGE, FONTS_LAST};
namespace gl3 {

class GuiHandler: public engine::ecs::System {
    using event_t = engine::events::Event<GuiHandler>;
    static event_t onChangeToCombatScene;
public:
    GuiHandler(engine::Game &game );
    ~GuiHandler();
    void initialize(engine::Game &game);
    static struct nk_image getTileImage(
        GLuint texID, int tileX,
        int tileY, int tileWidth,
        int tileHeight, int atlasWidth, int atlasHeight
    );
    [[nodiscard]] std::vector<nk_font*>& GetFonts() {
        return m_Fonts;
    }
private:
    void restoreGameState();
    void selectCurrentScene(engine::Game &game);
    void renderGUI(engine::Game &game);


    void setStyleWindow(nk_style* style);
    void setStyleButton(nk_style* style);
    void setStyleOptionLabel(nk_style* style);
    void setStyleProgressBar(nk_style* style);
    void setStyleSlider(nk_style* style);
    void setStyleText(nk_style* style);
    //-----GUI Szenen-----
    GuiScene activeScene;
    std::shared_ptr<engine::ecs::Gui> activeGui = nullptr;

    int windowWidth, windowHeight= 0;
    int currentW, currentH = 720;
    nk_font_atlas *atlas = nullptr;
    GLuint textureAtlasID = -1;
    GLFWwindow* window;
    nk_context *nkCTX;
    nk_glfw glfw = {nullptr};
    std::vector<nk_font*> m_Fonts = {};

};

} // gl3