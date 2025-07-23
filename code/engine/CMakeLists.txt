cmake_minimum_required(VERSION 3.29)

file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "include/**.h")
file(GLOB_RECURSE PRIVATE_HEADER_LIST CONFIGURE_DEPENDS "src/**.h")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "src/**.cpp")

set(ENGINE_NAME Engine)

add_library(${ENGINE_NAME} ${HEADER_LIST} ${PRIVATE_HEADER_LIST} ${SOURCE_LIST}
        include/engine/ecs/Gui.h
        src/logic/ActionRegistry.cpp
        include/engine/logic/ActionRegistry.h
        include/engine/logic/Action.h
        include/engine/logic/Action.h
        include/engine/Texture.h
        src/Texture.cpp
        include/engine/util/VertPresets.h
        src/sceneGraph/Transform.cpp
        include/engine/sceneGraph/Transform.h
        include/engine/sceneGraph/SceneGraphPruner.h
        include/engine/sceneGraph/SceneGraphUpdater.h
        include/engine/util/Debug.h
)

target_compile_features(${ENGINE_NAME} PRIVATE cxx_std_17)
set_target_properties(${ENGINE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${ENGINE_NAME} PUBLIC glad
        glfw
        glm
        soloud
        nuklear_impl
        stb
)

target_include_directories(${ENGINE_NAME} PUBLIC include/
        ${CMAKE_SOURCE_DIR}/extern/nlohmann)

