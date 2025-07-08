cmake_minimum_required(VERSION 3.18)

# set variables for source files
set(SOURCE_FILES src/main.cpp
        src/Game.cpp
        src/Game.h
        src/rendering/Shader.cpp
        src/rendering/Shader.h
        src/rendering/Mesh.cpp
        src/rendering/Mesh.h
        src/entities/Entity.cpp
        src/entities/Entity.h
        src/entities/Ship.cpp
        src/entities/Ship.h
        src/Assets.h
        src/entities/Planet.cpp
        src/entities/Planet.h
        src/entities/Enemy.cpp
        src/entities/Enemy.h
        src/entities/Missile.cpp
        src/entities/Missile.h
#        src/guiElements/guiElement.cpp
#        src/guiElements/guiElement.h
#        src/guiElements/battleMenu.cpp
#        src/guiElements/battleMenu.h
#        src/rendering/pngTexture.cpp
#        src/rendering/pngTexture.h
        src/combatLogic/Army.cpp
        src/combatLogic/Army.h
        src/components/unitTypes/Unit.h
        src/systems/CombatController.cpp
        src/systems/CombatController.h
        src/gui/guiCombat.cpp
        src/gui/guiCombat.h
        src/components/unitTypes/Infantry.h
        src/components/unitTypes/Archer.h
        src/components/unitTypes/Catapult.h
        src/systems/GuiHandler.cpp
        src/systems/GuiHandler.h
        src/components/GuiState.h
        src/components/CombatSelection.h
)

# set executable name
set(EXE_FILE Game)

# add the executable target
add_executable(${EXE_FILE} ${SOURCE_FILES})

# require C++ 17 compiler
target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)


find_package(imgui REQUIRED)
find_package(PNG REQUIRED)

target_link_libraries(${EXE_FILE} PRIVATE Engine)

target_compile_definitions(${EXE_FILE} PRIVATE ASSET_ROOT=./assets/)

# copy assets to output directory before build
add_custom_command(TARGET ${EXE_FILE} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXE_FILE}>/assets)


