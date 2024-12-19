cmake_minimum_required(VERSION 3.18)

# Add the Nuklear implementation file
set(NUKLEAR_IMPL_SRC nuklear_impl.c)

# Create a static library for Nuklear
add_library(nuklear_impl STATIC ${NUKLEAR_IMPL_SRC})

# Include the Nuklear submodule headers
target_include_directories(nuklear_impl PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../nuklear
)
