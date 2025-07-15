cmake_minimum_required(VERSION 3.29)



project(code)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(extern/glad)

add_subdirectory(extern/glfw)

add_subdirectory(extern/glm)

add_subdirectory(extern/soloud)

add_subdirectory(extern/tinygltf)

add_subdirectory(extern/custom_implementation/nuklear)

add_subdirectory(extern/custom_implementation/stb)

add_subdirectory(engine)

add_subdirectory(game)
