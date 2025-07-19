cmake_minimum_required(VERSION 3.18)

# set variables for source files
set(SOURCE_FILES src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Assets.h
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
        src/components/unitTypes/SiegeEngine.h
        src/components/Model2D.h
        src/components/Shader.h
        src/systems/rendering/RenderSystem.cpp
        src/systems/rendering/RenderSystem.h
        src/components/InstanceBuffer.h
        src/components/UnitState.h
        src/systems/rendering/InstanceManager.cpp
        src/systems/rendering/InstanceManager.h
        src/logic/CombatFunctions.cpp
        src/logic/CombatFunctions.h
)

# set executable name
set(EXE_FILE Game)

# add the executable target
add_executable(${EXE_FILE} ${SOURCE_FILES})

# require C++ 17 compiler
target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)



target_link_libraries(${EXE_FILE} PRIVATE Engine)

target_compile_definitions(${EXE_FILE} PRIVATE ASSET_ROOT=./assets/)

# copy assets to output directory before build
add_custom_command(TARGET ${EXE_FILE} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXE_FILE}>/assets)


