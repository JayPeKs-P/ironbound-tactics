//
// Created by Julian Kalb on 16/07/2025.
//

#pragma once

namespace gl3::engine::util {
class Texture
{
public:
    static unsigned int load(const char* texturePath, bool flip = true);
};
};
