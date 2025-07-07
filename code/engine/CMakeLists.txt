cmake_minimum_required(VERSION 3.29)

file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "include/**.h")
file(GLOB_RECURSE PRIVATE_HEADER_LIST CONFIGURE_DEPENDS "src/**.h")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "src/**.cpp")

set(ENGINE_NAME Engine)

add_library(${ENGINE_NAME} ${HEADER_LIST} ${PRIVATE_HEADER_LIST} ${SOURCE_LIST}
        include/engine/ecs/Gui.h
)

target_compile_features(${ENGINE_NAME} PRIVATE cxx_std_17)
set_target_properties(${ENGINE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${ENGINE_NAME} PUBLIC glad
        glfw
        glm
        soloud
        tinygltf
        nuklear_impl
        stb)

target_include_directories(${ENGINE_NAME} PUBLIC include/)

