#include <iostream>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "Game.h"

int main()
{
    try
    {
        gl3::Game spaceBattle(1280, 720, "Space Battle");
        spaceBattle.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unhandled exception" << e.what() << std::endl;
    }
}
