# How to make a new game with my codebase

## 1) Create your own "Game" class

- make your "Game" class inherit from the abstract "Game" class located in `engine/include/engine/Game.h`
- specify the dimensions of your applications window and its name and initialize engine/Game.h with those parameters

```cpp
#include "engine/Game.h"

class Game: public engine::Game {
public:
    Game(int width, int height, const std::string& title);

private:
    void start() override;
    void update(GLFWwindow *window) override;
    void draw() override;
}
```
- override the abstract functions `start()`, `update()` and `draw()` to your liking
  - `start()` is called once before the update loop
  - `update()` is called before `draw()`
- you do not need to manually call these functions
- `engine/Game.h` has a function called `run()` in your entrypoint create an object of your "Game" class and call `run()` on it

```cpp
int main()
{
    gl3::Game ironBoundTactics(1280, 720, "Ironbound Tactics");
    ironBoundTactics.run();
}

```

## 2) Add new entities

- this engine uses an ECS
- to create a new entity to the following, for example in your overwritten `Game.start()` function:

```cpp
start(){
    auto& entity = engine::Game::entityManager.createEntity();
}
```
 Some basic components are part of the code base, for example if you load a texture by using
 one of the utility functions `AddTextureToRegistry()`, store its ID and then add a Model2D component, a Shader component
 and a Transform component. The Model2D component needs a `std::vector<float>` with a specific 
formating. Therefore, some Presets are stored in `engine/include/engine/util/VertPresets.h`.
 
On executing your program the loaded Texture will be displayed on a quad by the RenderSystem.h

```cpp
#include "engine/rendering/Shader.h"
#include "engine/rendering/Model2D.h"
#include "engine/Texture.h"
#include "engine/util/VertPresets.h"

start(){
    AddTextureToRegistry("assets/textures/MyTexture.png", "Title");
    
    auto& titleLogo_E = engine::Game::entityManager.createEntity();
    auto tempTexID = GetTextureFromRegistry("Title");
    titleLogo_E.addComponent<Model2D>(VertPreset::title, VertPreset::quadIndices, tempTexID);
    titleLogo_E.addComponent<Shader>();
    auto& titleTransform = titleLogo_E.addComponent<Transform>(origin, glm::vec3(0, 0.5, -0.51), 0, glm::vec3(1.5, 1.5, 1));
}
```
In case you want to add entities, which use the same Texture add an additional InstanceBuffer component to it.
If you now add any new entities, give them a Transform component and register the existing entity as the parent transform,
all children will automatically be rendered using a single draw-call. 

## 3) Adding Sound

The SoundSystem is a singleton. To add a sound to its registry just get the single instance pointer as follows.

```cpp
#include "engine/SoundSystem.h"

start(){
    
    ...
    
    auto pSoundSystem = SoundSystem::GetInstance();
}
```
To register an audio file call either `AddSound()` or `AddMusic()`. Those functions require an identifier in their constructor
which will later be used to reference the sound effect. `SoundSystem.h` has an enum with existing identifiers. Feel free
to extend it.
Then at the desired point in your code call either `PlayMusic()` or `PlaySound()`.
```cpp
#include "engine/SoundSystem.h"

Game::Game(int width, int height, const std::string& title):
    engine::Game(width, height, title) {
    auto pSoundSystem = engine::SoundSystem::GetInstance();
    pSoundSystem->RegisterMusic(engine::MUSIC_MAIN_MENU, "Logo #2 (Looped).wav");

start(){
    
    ...
    
    auto pSoundSystem = SoundSystem::GetInstance();
    pSoundSystem.PlayMusic(engine::MUSIC_MAIN_MENU);
}
```

## 4) Extend the ECS with your own Components and Systems
If you want to create a new type of component use the following pattern:

```cpp
#include "engine/ecs/Component.h"

using gl3::engine::ecs::Component;
using gl3::engine::ecs::ComponentManager;
using gl3::engine::ecs::guid_t;
using gl3::engine::ecs::Entity;

struct MyComponent : Component {
    friend ComponentManager;
    friend Entity;

private:
    MyComponent(guid_t owner) : Component(owner) {
    }
};
```
If you do this it will without any problem be usable like demonstrated in step **2)**.
You will also want to create new Systems to add functionality to your game. Create new Systems like this:
```cpp
#include "engine/ecs/ecs.h"
#include "engine/ecs/System.h"


namespace gl3 {
    class MySystem: public engine::ecs::System {
    public:
        explicit MySystem(engine::Game& game);
    private:
        void ExampleFunction();
    };
}
```
Systems always need to be passed a reference to your game in their constructor. This gives them
access to core Events. `engine/Game.h` has the following events which are `invoked()` once each,
either before,during *(each time)* or after the update loop:

```cpp
        event_t onStartup;
        event_t onAfterStartup;
        event_t onBeforeUpdate;
        event_t onUpdate;
        event_t onAfterUpdate;
        event_t onBeforeShutdown;
        event_t onShutdown;
```
Your system can access them as follows and register any function as a callback:

```cpp
#include MySystem.h

MySystem::MySystem(engine::Game& game):
    System(game) {
    engine.onAfterStartup.addListener([&](engine::Game &game)
    {
        ExampleFunction();
    });
}
```
Now `ExampleFunction()` will be called once before the game loop each time you start your application.
