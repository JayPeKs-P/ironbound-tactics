//
// Created by Julian Kalb on 16/07/2025.
//

#pragma once

namespace gl3::engine::util {
class Texture
{
public:
    /// @brief Helper Function to load a texture in OpenGL
    ///
    /// @param texturePath Full path to the file.
    /// @param flip If false does not flip the texture vertically. Necessary for Nuklear.
    static unsigned int load(const char* texturePath, bool flip = true);
};
};
