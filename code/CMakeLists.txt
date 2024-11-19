cmake_minimum_required(VERSION 3.29)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

project(code)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(extern/glad)

add_subdirectory(extern/glfw)

add_subdirectory(extern/glm)

add_subdirectory(extern/soloud)

add_subdirectory(extern/tinygltf)

add_subdirectory(game)
