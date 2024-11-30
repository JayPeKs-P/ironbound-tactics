//
// Created by julia on 30/11/2024.
//

#pragma once



class Texture {
public:
    [[nodiscard]] GLuint getTexture_id() const
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

    explicit PngTexture(const fs::path& relativeAssetPath);

    ~PngTextureLoader();
    void loadPngTexture();

private:
    FILE* file;
    fs::path fullPath;
    GLuint textureID = 0;
    int width = 0;
    int height = 0;

};
