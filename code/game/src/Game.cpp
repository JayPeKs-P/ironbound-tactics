//
// Created by julia on 22/10/2024.
//

#include "Game.h"

#include <random>                           // for std::mt19937, std::uniform_int_distribution and std::random_device

#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include "Assets.h"
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
    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        nk_glfw3_font_stash_end(&glfw);
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        /*nk_style_set_font(ctx, &droid->handle);*/}
}

void Game::updateGUI()
{
    nk_glfw3_new_frame(&glfw);
}

void Game::drawGUI()
{

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    /* GUI */
    if (nk_begin(ctx, "Army",
        nk_rect(windowWidth/4,
        windowHeight - windowHeight/4,
        windowWidth/2,
        windowHeight/4),
        NK_WINDOW_TITLE))
    {
        enum {Infantry, Archer};
        enum {Catapult, Assault_Cover};

        static int humanoid = -1;
        static int siege = -1;

        int amountOfTroups = 10;
        ;
        static int value = 1;

        nk_layout_row_dynamic(ctx, windowHeight/6, 2);
        if (nk_group_begin(ctx, "UnitsGroup", NK_WINDOW_BORDER))
        {
            nk_label(ctx, "Units", NK_TEXT_CENTERED);
            if (nk_tree_push(ctx, NK_TREE_TAB, "Infantry", NK_MINIMIZED))
            {
                nk_layout_row_push(ctx, 50);
                nk_label(ctx, "Volume:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 110);
                nk_slider_int(ctx, 0, &value, amountOfTroups, 1);
                nk_tree_pop(ctx);
            }
            if (nk_tree_push(ctx, NK_TREE_TAB, "Archers", NK_MINIMIZED))
            {
                nk_layout_row_push(ctx, 50);
                nk_label(ctx, "Volume:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 110);
                nk_slider_int(ctx, 0, &value, amountOfTroups, 1);
                nk_tree_pop(ctx);
            }
            nk_group_end(ctx);
        }
        if (nk_group_begin(ctx, "SiegeGroup", NK_WINDOW_BORDER))
        {
            if (nk_tree_push(ctx, NK_TREE_TAB, "Siege", NK_MINIMIZED))
            {
                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Volume:", NK_TEXT_LEFT);
                nk_slider_int(ctx, 0, &value, amountOfTroups, 1);
                nk_tree_pop(ctx);
            }
            nk_group_end(ctx);
        }

        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_option_label(ctx, "Infantry", humanoid == Infantry))
        {
            humanoid = Infantry;
            amountOfTroups = 20;
        }
        if (nk_option_label(ctx, "Archer", humanoid == Archer))
        {
            humanoid = Archer;
            amountOfTroups = 10;
        }
        if (nk_tree_push(ctx, NK_TREE_NODE, "Siege Units", NK_MINIMIZED))
        {
            // nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_option_label(ctx, "Catapult", siege == Catapult))
            {
                siege = Catapult;
            }
            if (nk_option_label(ctx, "Assault Cover", siege == Assault_Cover))
            {
                siege = Assault_Cover;
            }
            nk_tree_pop(ctx);
        }


        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "button"))
            fprintf(stdout, "button pressed\n");

        nk_layout_row_dynamic(ctx, 25, 1);
        // nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 1);
        // if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))) {
        //     nk_layout_row_dynamic(ctx, 120, 1);
        //     bg = nk_color_picker(ctx, bg, NK_RGBA);
        //     nk_layout_row_dynamic(ctx, 25, 1);
        //     bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
        //     bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
        //     bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
        //     bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
        //     nk_combo_end(ctx);
        // }
    }
    nk_end(ctx);

    if (nk_begin(ctx, "Enemy",
        nk_rect(windowWidth/4,
        0,
        windowWidth/2,
        windowHeight/4),
        NK_WINDOW_NO_INPUT|NK_WINDOW_TITLE))
    {
        enum {Infantry, Archer, Siege};
        static int type = -1;

        nk_layout_row_dynamic(ctx, 30, 3);
        if (nk_option_label(ctx, "infantry", type == Infantry))
        {
            type = Infantry;
        }
        if (nk_option_label(ctx, "archer", type == Archer))
        {
            type = Archer;
        }
        if (nk_option_label(ctx, "siege", type == Siege))
        {
            type = Siege;
        }


    }
    nk_end(ctx);


    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON,
            MAX_VERTEX_BUFFER,
            MAX_ELEMENT_BUFFER);
}

void Game::destroyGUI()
{
    nk_glfw3_shutdown(&glfw);
}

