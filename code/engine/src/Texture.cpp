//
// Created by Julian Kalb on 16/07/2025.
//
#include "engine/Texture.h"

#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace gl3::engine::util;
unsigned int Texture::load(const char* texturePath)
{
    unsigned int textureID;
    int width, height, channels;
    unsigned char* image = stbi_load(texturePath, &width, &height, &channels, 4); // Load as RGBA

    if (!image) {
        std::cerr << "Failed to load texture atlas: " << texturePath << "\n";
        return -1;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}