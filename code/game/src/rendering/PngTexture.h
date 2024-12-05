//
// Created by julia on 30/11/2024.
//

#pragma once
#include <string>

#include <png.h>
#include "../Assets.h"
#include "glad/glad.h"


class PngTexture {
public:
    [[nodiscard]] GLuint getTextureID() const
    {
        return textureID;
    }

    [[nodiscard]] int getPngWidth() const
    {
        return width;
    }

    [[nodiscard]] int getPngHeight() const
    {
        return height;
    }

    explicit PngTexture(const std::filesystem::path& relativeTexturePath);

    ~PngTexture();
    PngTexture(const PngTexture &other) = delete;
    PngTexture(PngTexture &&other) noexcept
    {
        std::swap(textureID, other.textureID);
        std::swap(width, other.width);
        std::swap(height, other.height);
    }
    void loadPngTexture(FILE* file);

private:
    GLuint textureID = 0;
    int width = 0;
    int height = 0;

};
