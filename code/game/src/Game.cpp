//
// Created by julia on 22/10/2024.
//

#include "Game.h"

#include <charconv>
#include <random>                           // for std::mt19937, std::uniform_int_distribution and std::random_device

#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Assets.h"
#include "entities/unitTypes/UnitHumanoid.h"
// #include "entities/Enemy.h"
// #include "entities/Planet.h"


using namespace gl3;

Game::Game(int width, int height, const std::string &title)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    // define openGL version (4.6)
    //--------------------------------
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window == nullptr)
    {
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    if(glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("gl error");
    }
    // audio.init();
    // audio.setGlobalVolume(0.1f);
    initGUI();

}

Game::~Game()
{
    glfwTerminate();
    destroyGUI();
}

glm::mat4 Game::calculateMvpMatrix(glm::vec3 position, float zRotationDegrees, glm::vec3 scale)
{
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(zRotationDegrees), glm::vec3(0, 0, 1));

    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 90.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::perspective(glm::radians(2.0f),1000.0f/600.0f,0.1f,100.0f);

    glm::mat4 mvpMatrix = projection * view * model;
    return mvpMatrix;
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::update()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    for (const auto & entity : entities)
    {
        entity->update(this, deltaTime);
    }
    updateGUI();
}

void Game::draw()
{
    glClearColor(0.172f, 0.243f, 0.313f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto & entity : entities)
    {
        entity->draw(this);
    }
    drawGUI();

     glfwSwapBuffers(window);
}

void Game::updateDeltaTime()
{
    float frameTime = glfwGetTime();
    deltaTime = frameTime - lastFrameTime;
    lastFrameTime = frameTime;
}

void Game::run()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    std::mt19937 randomNumberEngine{       //Mersenne Twister generates 32-bit unsigned integers
        std::random_device{}()};       // Seed our Mersenne Twister using with a random number from the OS
    std::uniform_real_distribution positionDist{-2.0f,2.0f};
    std::uniform_real_distribution scaleDist{0.05f,0.2f};
    std::uniform_real_distribution colorDist{0.0f,1.0f};
    // for(auto i = 0; i < 100; ++i) {
    //     glm::vec3 randomPosition = {positionDist(randomNumberEngine) * 1.5f,
    //                                 positionDist(randomNumberEngine) * 1.5f,
    //                                 0.0f};
    //     float randomScale = scaleDist(randomNumberEngine);
    //     auto rgbValue = colorDist(randomNumberEngine);      //Used to guarantee, that all planets are a shade of grey.
    //     glm::vec4 randomColor = {rgbValue,
    //                              rgbValue,
    //                              rgbValue,
    //                              1.0};
    //
    //     auto entity = std::make_unique<Planet>(randomPosition, randomScale, randomColor);
    //     entities.push_back(std::move(entity));
    // }

    // auto spaceShip = std::make_unique<Ship>(glm::vec3(-2, 1, 0));
    // ship = spaceShip.get();
    // entities.push_back(std::move(spaceShip));
    //
    // auto enemy = std::make_unique<Enemy>(glm::vec3(2, -1, 0), -90.0f, 0.25f);
    // entities.push_back(std::move(enemy));

    backgroundMusic = std::make_unique<SoLoud::Wav>();
    backgroundMusic->load(resolveAssetPath("audio/electronic-wave.mp3").string().c_str());
    backgroundMusic->setLooping(true);
    audio.playBackground(*backgroundMusic);

    glfwSetTime(1.0/60);

    while (!glfwWindowShouldClose(window))
    {
        update();
        draw();
        updateDeltaTime();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
}

void Game::initGUI()
{
    loadTextureAtlas("assets/textures/gui/ui_atlas_48x48.png");
    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    style = &ctx->style;

    style->window.fixed_background = nk_style_item_image(getTileImage(2, 54, 1, 1, 3072, 3072));
    style->window.border = 10.0f;
    style->window.border_color = nk_rgba(120, 58, 58, 163);
    style->window.header.label_normal = nk_rgba(120, 58, 58, 210);
    style->window.header.label_hover = nk_rgba(120, 58, 58, 210);
    style->window.header.label_active = nk_rgba(120, 58, 58, 210);
    style->window.header.normal = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
    style->window.header.hover = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
    style->window.header.active = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));

    style->button.normal = nk_style_item_image(getTileImage(0, 11, 3, 1, 3072, 3072));
    style->button.hover = nk_style_item_image(getTileImage(3, 11, 3, 1, 3072, 3072));
    style->button.active = nk_style_item_image(getTileImage(6, 11, 3, 1, 3072, 3072));
    style->button.text_normal = nk_rgba(255, 255, 255, 255);
    style->button.text_hover = nk_rgba(255, 250, 200, 255);

    style->option.normal = nk_style_item_image(getTileImage(21, 11, 1, 1, 3072, 3072));
    style->option.hover = nk_style_item_image(getTileImage(22, 11, 1, 1, 3072, 3072));
    style->option.active = nk_style_item_image(getTileImage(23, 11, 1, 1, 3072, 3072));
    style->option.cursor_normal = nk_style_item_image(getTileImage(24, 11, 1, 1, 3072, 3072));
    style->option.cursor_hover = nk_style_item_image(getTileImage(25, 11, 1, 1, 3072, 3072));
    style->option.text_normal = nk_rgba(255, 255, 255, 255);
    style->option.text_hover = nk_rgba(255, 250, 200, 255);

    style->progress.normal = nk_style_item_image(getTileImage(0, 14, 3, 1, 3072, 3072));
    style->progress.cursor_normal = nk_style_item_image(getTileImage(6, 14, 3, 1, 3072, 3072));

    style->slider.normal = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.hover = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.active = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.cursor_normal = nk_style_item_image(getTileImage(21, 12, 1, 1, 3072, 3072));
    style->slider.cursor_hover = nk_style_item_image(getTileImage(22, 12, 1, 1, 3072, 3072));
    style->slider.cursor_active = nk_style_item_image(getTileImage(23, 12, 1, 1, 3072, 3072));
    style->slider.bar_filled = nk_rgba(100, 100, 200, 200);
    style->slider.bar_normal = nk_rgba(92, 58, 58, 163);
    style->slider.bar_hover = nk_rgba(92, 58, 58, 163);
    style->slider.bar_active = nk_rgba(92, 58, 58, 163);

    style->text.color = nk_rgba(255, 255, 255, 255);

    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas);
        struct nk_font *FantasyRPG1 = nk_font_atlas_add_from_file(atlas, "assets/textures/gui/FantasyRPG1.ttf", 20, 0);
        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        nk_glfw3_font_stash_end(&glfw);
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &FantasyRPG1->handle);}
}

void Game::updateGUI()
{
    nk_glfw3_new_frame(&glfw);
}

void Game::drawPlayerHealthBars(struct nk_context *ctx, int windowWidth, int windowHeight) {

        nk_layout_row_dynamic(ctx, 24, 3);
        nk_label(ctx, "Infantry", NK_TEXT_LEFT);
        nk_label(ctx, "Archer", NK_TEXT_LEFT);
        nk_label(ctx, "Siege", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 36, 3);
        nk_progress(ctx, &healthInfantryPlayer, 100, NK_FIXED);
        nk_progress(ctx, &healthArcherPlayer, 100, NK_FIXED);
        nk_progress(ctx, &healthSiegePlayer, 100, NK_FIXED);
}

void Game::drawUnitSelectionMenu(struct nk_context *ctx, int windowWidth, int windowHeight) {
    if (nk_begin(ctx, "Units",
        nk_rect(windowWidth / 4, windowHeight - windowHeight / 2, windowWidth / 2, 60), NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {

        nk_layout_row_dynamic(ctx, 36, 3);

        const char* unitNames[] = { "Infantry", "Archer", "Siege" };
        Category unitCategories[] = { Category::Infantry, Category::Archer, Category::Siege };
        int unitTroops[] = { infAmount, archAmount, siegeAmount };

        for (int i = 0; i < 3; i++) {
            if (nk_button_label(ctx, unitNames[i])) {
                if (selectedOne == Category::Empty) {
                    selectedOne = unitCategories[i];
                    amountOfTroups = unitTroops[i];
                    std::cout << "Selected " << unitNames[i] << " first\n";
                } else if (selectedOne != unitCategories[i]) {
                    selectedTwo = unitCategories[i];
                    owner = OwnerOfUnit::Player;
                    std::cout << "Selected " << unitNames[i] << " second\n";
                } else {
                    selectedOne = Category::Empty;
                    selectedTwo = Category::Empty;
                    owner = OwnerOfUnit::No_Selection;
                    std::cout << "Reset to empty\n";
                }
            }
        }
    }
    nk_end(ctx);
}

void Game::drawUnitActions(struct nk_context *ctx, Category selectedUnit)  {
    if (selectedUnit == Category::Empty) return;

    nk_layout_row_dynamic(ctx, 26, 3);
    nk_label(ctx, (selectedUnit == Category::Infantry) ? "Infantry" :
                   (selectedUnit == Category::Archer) ? "Archer" : "Siege", NK_TEXT_LEFT);
    nk_label(ctx, "Available  Units: ", NK_TEXT_LEFT);

    char amountLabel[16];
    snprintf(amountLabel, sizeof(amountLabel), "%d", amountOfTroups);
    nk_label(ctx, amountLabel, NK_TEXT_LEFT);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Available  Actions", NK_MAXIMIZED)) {
        nk_layout_row_dynamic(ctx, 36, 2);

        if (owner == OwnerOfUnit::AI && selectedTwo != Category::Empty) {
            if (nk_button_label(ctx, "Attack"))
            {
                if (selectedOne == Category::Archer && round == ONE) amountOfTroups = 0;
                if (selectedOne == Category::Siege) amountOfTroups = 0, reset();
                if (selectedOne == Category::Archer && round == TWO) amountOfTroups = 0, reset();

            }
            nk_slider_int(ctx, 0, &valueAttack, amountOfTroups, 1);
        } else if (selectedTwo == Category::Siege) {
            if (nk_button_label(ctx, "Use Catapult"))
            {
                if (selectedOne == Category::Infantry) amountOfTroups = 20;
            }
            nk_slider_int(ctx, 0, &valueCatapult, amountOfTroups, 1);
            if (nk_button_label(ctx, "Use Assault Cover"))
            {
                if (selectedOne == Category::Infantry) amountOfTroups = 0;
            }
            nk_slider_int(ctx, 0, &valueAssaultCover, amountOfTroups, 1);
        } else if (selectedTwo == Category::Archer) {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == Category::Siege) amountOfTroups = 6;

            }
            nk_slider_int(ctx, 0, &valueDefend, amountOfTroups, 1);
        }else if (selectedTwo == Category::Infantry)
        {
            if (nk_button_label(ctx, "Protect"))
            {
                if (selectedOne == Category::Siege) amountOfTroups = 0;
            }
            nk_slider_int(ctx, 0, &valueDefendInf, amountOfTroups, 1);
        }
        nk_tree_pop(ctx);
    }
}

void Game::reset()
{
    if (round == ONE)
    {
        selectedOne = Category::Empty;
        selectedTwo = Category::Empty;
        owner = OwnerOfUnit::No_Selection;

        infAmount = 18;
        archAmount = 28;
        siegeAmount = 6;

        healthInfantryPlayer = 85;
        healthArcherPlayer = 93;
        healthSiegePlayer = 100;

        healthInfantryAI = 100;
        healthArcherAI = 75;
        healthSiegeAI = 100;

        round = TWO;
    }
    else if (round == TWO)
    {
        selectedOne = Category::Empty;
        selectedTwo = Category::Empty;
        owner = OwnerOfUnit::No_Selection;

        infAmount = 29;
        archAmount = 26;
        siegeAmount = 7;

        healthInfantryPlayer = 72;
        healthArcherPlayer = 86;
        healthSiegePlayer = 70;

        healthInfantryAI = 45;
        healthArcherAI = 75;
        healthSiegeAI = 50;
    }
}


void Game::drawEnemyHealthBars(struct nk_context *ctx, int windowWidth, int windowHeight) {
    if (nk_begin(ctx, "Enemy",
        nk_rect(windowWidth / 4, 0, windowWidth / 2, windowHeight / 4),
        NK_WINDOW_TITLE|NK_WINDOW_BORDER)) {

        nk_layout_row_dynamic(ctx, 36, 3);
        nk_progress(ctx, &healthInfantryAI, 100, NK_FIXED);
        nk_progress(ctx, &healthArcherAI, 100, NK_FIXED);
        nk_progress(ctx, &healthSiegeAI, 100, NK_FIXED);

        nk_layout_row_dynamic(ctx, 36, 3);
        const char* enemyNames[] = { "Infantry", "Archer", "Siege" };
        Category enemyCategories[] = { Category::Infantry, Category::Archer, Category::Siege };

        for (int i = 0; i < 3; i++) {
            if (nk_option_label(ctx, enemyNames[i], owner == OwnerOfUnit::AI && selectedTwo == enemyCategories[i]) && selectedOne != Category::Empty) {
                if (owner != OwnerOfUnit::AI) owner = OwnerOfUnit::AI;
                if (selectedTwo != enemyCategories[i]) selectedTwo = enemyCategories[i];
            }
        }
    }
    nk_end(ctx);
}

void Game::drawRender(struct nk_context *ctx, int windowWidth, int windowHeight) {
    drawUnitSelectionMenu(ctx, windowWidth, windowHeight);

    if (nk_begin(ctx, "UnitsGroup", nk_rect(windowWidth / 4, windowHeight - windowHeight / 3, windowWidth / 2, windowHeight / 3), NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
        drawPlayerHealthBars(ctx, windowWidth, windowHeight);
        drawUnitActions(ctx, selectedOne);
    }
    nk_end(ctx);

    drawEnemyHealthBars(ctx, windowWidth, windowHeight);

    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void Game::drawGUI()
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    drawRender(ctx, windowWidth, windowHeight);
}

void Game::destroyGUI()
{
    nk_glfw3_shutdown(&glfw);
}


void Game::loadTextureAtlas(const char* texturePath)
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

    // Convert OpenGL texture to Nuklear Image Handle
    atlas = nk_image_id(textureAtlasID);
}

struct nk_image Game::getTileImage(int tileX, int tileY, int tileWidth, int tileHeight, int atlasWidth, int atlasHeight)
{
    return nk_subimage_id(textureAtlasID, atlasWidth, atlasHeight,
                          nk_rect(tileX * 48, tileY * 48, tileWidth * 48, tileHeight * 48));
}
