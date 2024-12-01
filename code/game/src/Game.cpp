//
// Created by julia on 22/10/2024.
//

#include "Game.h"

#include <random>                           // for std::mt19937, std::uniform_int_distribution and std::random_device

#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include "Assets.h"
#include "entities/Enemy.h"
#include "entities/Planet.h"


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
    audio.init();
    audio.setGlobalVolume(0.1f);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // Capture user input and configurations
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    io.Fonts->AddFontFromFileTTF("C:\\Users\\julia\\Git-projects\\05-gl3-kalb\\code\\assets\\textures\\gui\\ui_atlas_48x48.png", 20);
    // ImGui::StyleColorsDark(); // Set the default ImGui style
}

Game::~Game()
{
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

}

void Game::draw()
{
    glClearColor(0.172f, 0.243f, 0.313f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto & entity : entities)
    {
        entity->draw(this);
    }
    battleMenu->renderBattleMenu();
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
    for(auto i = 0; i < 100; ++i) {
        glm::vec3 randomPosition = {positionDist(randomNumberEngine) * 1.5f,
                                    positionDist(randomNumberEngine) * 1.5f,
                                    0.0f};
        float randomScale = scaleDist(randomNumberEngine);
        auto rgbValue = colorDist(randomNumberEngine);      //Used to guarantee, that all planets are a shade of grey.
        glm::vec4 randomColor = {rgbValue,
                                 rgbValue,
                                 rgbValue,
                                 1.0};

        auto entity = std::make_unique<Planet>(randomPosition, randomScale, randomColor);
        entities.push_back(std::move(entity));
    }

    auto spaceShip = std::make_unique<Ship>(glm::vec3(-2, 1, 0));
    ship = spaceShip.get();
    entities.push_back(std::move(spaceShip));

    auto enemy = std::make_unique<Enemy>(glm::vec3(2, -1, 0), -90.0f, 0.25f);
    entities.push_back(std::move(enemy));

    backgroundMusic = std::make_unique<SoLoud::Wav>();
    backgroundMusic->load(resolveAssetPath("audio/electronic-wave.mp3").string().c_str());
    backgroundMusic->setLooping(true);
    audio.playBackground(*backgroundMusic);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    ImVec2 windowSize(width, height);
    auto menu = std::make_unique<BattleMenu>(windowSize);
    battleMenu = menu.get();

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


