//
// Created by Julian Kalb on 22/10/2024.
//
#pragma once
#include <glad/glad.h>
#include "../src/entities/Entity.h"


#include <soloud.h>
#include <soloud_wav.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


//-----------------------------------------
/*
NOTE TO MYSELF: Should there ever be any
weird behavior with rendering, I never
fully understood those macros (they are
used in nk_glfw3_render(), which uses their
data for glBufferData()
*/
//-----------------------------------------
//!!!!! already set in tinygltf #define STB_IMAGE_IMPLEMENTATION !!!!!!!!
#include <stb_image.h>
#include "../src/gui/CombatController.h"

namespace gl3
{
    class Game {
    public:
        Game(int width, int height, const std::string& title);
        ~Game();
        static glm::mat4 calculateMvpMatrix(glm::vec3 position, float zRotationDegrees, glm::vec3 scale);

        void run();

        [[nodiscard]] GLFWwindow* getWindow() const
        {
            return window;
        }

    private:
        //--------------GUI--------------------------------
        // enum class OwnerOfUnit{No_Selection, Player, AI};
        // enum class Category {Empty, Infantry, Archer, Siege, Catapult, Assault_Cover};
        //
        // void initGUI();
        // void updateGUI();
        // void drawGUI();
        // void destroyGUI();
        //
        // void drawPlayerHealthBars(struct nk_context *ctx, int windowWidth, int windowHeight);
        // void drawUnitSelectionMenu(struct nk_context *ctx, int windowWidth, int windowHeight);
        // void drawUnitActions(struct nk_context *ctx, Category selectedUnit);
        // void drawEnemyHealthBars(struct nk_context *ctx, int windowWidth, int windowHeight);
        // void drawRender(struct nk_context *ctx, int windowWidth, int windowHeight);
        //
        //
        // OwnerOfUnit owner = OwnerOfUnit::No_Selection;
        // Category selectedOne = Category::Empty;
        // Category selectedTwo = Category::Empty;
        // int amountOfTroups = 0;
        // int valueAttack = 0;
        // int valueDefend = 0;
        // int valueCatapult = 0;
        // int valueAssaultCover = 0;
        // int valueDefendInf = 0;
        //
        // nk_size healthInfantryPlayer = 100;
        // nk_size healthArcherPlayer = 100;
        // nk_size healthSiegePlayer = 100;
        // nk_size healthInfantryAI = 100;
        // nk_size healthArcherAI = 100;
        // nk_size healthSiegeAI = 100;
        //
        // GLuint textureAtlasID;
        // struct nk_image atlas;
        // struct nk_style *style;
        // void loadTextureAtlas(const char* texturePath);
        //
        // struct nk_image getTileImage(int tileX, int tileY, int tileWidth, int tileHeight, int atlasWidth, int atlasHeight);
        // enum round{ONE, TWO, THREE, FOUR, FIVE, SIX};
        // int round = ONE;
        // int infAmount = 40;
        // int archAmount = 30;
        // int siegeAmount = 10;
        // void reset();
        //------------END OF GUI--------------------------------------
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        void update();
        void draw();
        void updateDeltaTime();



        std::vector<std::unique_ptr<Entity>> entities;

//        Ship* ship = nullptr;
        GLFWwindow* window = nullptr;
        glm::mat4 mvpMatrix{};

        // BattleMenu* battleMenu = nullptr;

        SoLoud::Soloud audio;
        std::unique_ptr<SoLoud::Wav>(backgroundMusic);

        float lastFrameTime{};
        float deltaTime{};

        float zRotation = 0.0f;
        float rotationSpeed = 120.0f;
        float xTranslate = 0.0f;
        float yTranslate = 0.0f;
        float translationSpeed = 1.0f;



    };
}
