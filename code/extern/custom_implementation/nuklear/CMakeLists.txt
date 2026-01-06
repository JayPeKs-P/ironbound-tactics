cmake_minimum_required(VERSION 3.18)


project(NUKLEAR C)

#add_library(nuklear_impl INTERFACE)
#
#target_include_directories(nuklear_impl INTERFACE
#${CMAKE_CURRENT_SOURCE_DIR}
#)
add_library(nuklear_impl STATIC
        nuklear_impl.cpp
        NuklearWrapper.h
)

target_link_libraries(nuklear_impl PUBLIC glfw glad)

target_include_directories(nuklear_impl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)