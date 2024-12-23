cmake_minimum_required(VERSION 3.18)
#
## Add the Nuklear implementation file
#set(NUKLEAR_IMPL_SRC nuklear_impl.c)
#
### Create a static library for Nuklear
##add_library(nuklear_impl STATIC ${NUKLEAR_IMPL_SRC})
#
### Include the Nuklear submodule headers
##target_include_directories(nuklear_impl PUBLIC
##        ${CMAKE_CURRENT_SOURCE_DIR}/../../nuklear
##)
project(NUKLEAR C)
## Path settings
#set(DEMO_PATH "${CMAKE_SOURCE_DIR}/extern/nuklear/demo")
#set(SRC_PATH "${CMAKE_SOURCE_DIR}/extern/nuklear/src")
#
## set nuklear header files variable
#set(HEADER_FILES ${CMAKE_SOURCE_DIR}/extern/Nuklear/nuklear.h
#        ${DEMO_PATH}/glfw_opengl3/nuklear_glfw_gl3.h
#)
#
#set(SOURCE_FILE "${SRC_PATH}/*.c")
## Create a static library for Nuklear
#add_library(nuklear_impl STATIC ${HEADER_FILES} ${SOURCE_FILE})
#
#target_compile_features(nuklear_impl PRIVATE cxx_std_17)
#
## Include the Nuklear submodule headers
#target_include_directories(nuklear_impl PUBLIC
#        ${CMAKE_SOURCE_DIR}/extern/Nuklear/
#        ${CMAKE_SOURCE_DIR}/extern/Nuklear/demo/glfw_opengl3
#)
#
add_library(nuklear_impl INTERFACE)

target_include_directories(nuklear_impl INTERFACE
${CMAKE_CURRENT_SOURCE_DIR}
)