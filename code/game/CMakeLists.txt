cmake_minimum_required(VERSION 3.18)

# set variables for source files
set(SOURCE_FILES src/main.cpp
        src/Game.cpp
        src/Game.h
        ../engine/include/engine/util/Assets.h
        src/components/unitTypes/Unit.h
        src/systems/CombatController.cpp
        src/systems/CombatController.h
        src/gui/guiCombat.cpp
        src/gui/guiCombat.h
        src/systems/GuiHandler.cpp
        src/systems/GuiHandler.h
        src/components/CombatSelection.h
        src/components/unitTypes/SiegeEngine.h
        ../engine/src/rendering/RenderSystem.cpp
        ../engine/include/engine/rendering/RenderSystem.h
        src/components/UnitState.h
        src/systems/rendering/InstanceManager.cpp
        src/systems/rendering/InstanceManager.h
        src/logic/LibCombatFunctions.cpp
        src/logic/LibCombatFunctions.h
        src/logic/EnemyAI.cpp
        src/logic/EnemyAI.h
        src/components/unitTypes/UnitCategory.h
        src/systems/SelectionSystem.h
        src/systems/MovementSystem.cpp
        src/systems/MovementSystem.h
        src/gui/GuiUnitSelection.cpp
        src/gui/GuiUnitSelection.h
        src/components/unitTypes/Unit.cpp
        src/components/Icon.h
        src/systems/HoverIconSystem.cpp
        src/systems/HoverIconSystem.h
        src/components/CombatSelection.cpp
)

# set executable name
set(EXE_FILE Game)

# add the executable target
add_executable(${EXE_FILE} WIN32 ${SOURCE_FILES})

# Removes console window in combination with ↑ WIN32 flag
target_link_options(${EXE_FILE} PRIVATE /ENTRY:mainCRTStartup)

#target_compile_definitions(${EXE_FILE} PRIVATE DEBUG_MODE)

# require C++ 20 compiler
target_compile_features(${EXE_FILE} PRIVATE cxx_std_20)



target_link_libraries(${EXE_FILE} PRIVATE Engine)

#target_compile_definitions(
#        ${EXE_FILE} PRIVATE
#)

# copy assets to output directory before build
add_custom_command(TARGET ${EXE_FILE} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXE_FILE}>/assets)


