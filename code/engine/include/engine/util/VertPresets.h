//
// Created by Julian Kalb on 17/07/2025.
//

#pragma once
#include <vector>

namespace gl3::engine::util::VertPreset
{
    static std::vector<float> quad =
        {
            //positions                  // colors                  // texture coords
             0.25f,   0.25f,   0.0f,      1.0f,   0.0f,   0.0f,       0.25f,   0.5f,
             0.25f,  -0.25f,   0.0f,      0.0f,   1.0f,   0.0f,       0.25f,   0.25f,
            -0.25f,  -0.25f,   0.0f,      0.0f,   0.0f,   1.0f,       0.0f,   0.25f,
            -0.25f,   0.25f,   0.0f,      1.0f,   1.0f,   0.0f,       0.0f,   0.5f
        };
    static std::vector<unsigned int> quadIndices = {0,1,3,1,2,3};

    static std::vector<float> background =
        {
            //positions                  // colors                  // texture coords
             1.0f,   0.574f,  -0.1f,      1.0f,   0.0f,   0.0f,       1.0f,   1.0f,     //top right
             1.0f,  -0.574f,  -0.1f,      0.0f,   1.0f,   0.0f,       1.0f,   0.0f,     //bottom right
            -1.0f,  -0.574f,  -0.1f,      0.0f,   0.0f,   1.0f,       0.0f,   0.0f,     //bottom left
            -1.0f,   0.574f,  -0.1f,      1.0f,   1.0f,   0.0f,       0.0f,   1.0f      //top left
        };
    static std::vector<unsigned int> backgroundIndices = {0,1,3,1,2,3};
    };