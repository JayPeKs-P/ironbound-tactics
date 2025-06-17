//
// Created by Julian Kalb on 30/11/2024.
//

#include "PngTexture.h"
#include <iostream>

using namespace gl3;

PngTexture::PngTexture(const std::filesystem::path& relativeTexturPath)
{
    auto fullPath = resolveAssetPath(relativeTexturPath);
    FILE* file = fopen(fullPath.string().c_str(), "rb");
    if (!file)
    {
        std::cerr << "Failed to open PNG file: " << fullPath << std::endl;
    }
    // Check PNG signature
    png_byte header[8];
    fread(header, 1, 8, file);
    if (png_sig_cmp(header, 0, 8)) {
        std::cerr << "File is not a valid PNG: " << fullPath << std::endl;
        fclose(file);
    }
    loadPngTexture(file);
}

PngTexture::~PngTexture()
{
    if (textureID != 0)
    {
        glDeleteTextures(1, &textureID);
    }
}

void PngTexture::loadPngTexture(FILE* file)
{
    // Initialize PNG structs
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
        nullptr,
        nullptr,
        nullptr);
    /*Check if the creation of the png_structp failed.
    If it failed, there might be insufficient memory,
    or libpng is not initialized correctly. */
    if (!png) {
        std::cerr << "Failed to create PNG read struct" << std::endl;
        fclose(file);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        std::cerr << "Failed to create PNG info struct" << std::endl;
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(file);
    }

    // Error handling
    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "Error during PNG creation" << std::endl;
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(file);
    }
    /*Libpng uses a non-standard error handling mechanism
     based on setjmp/longjmp. This ensures that errors
     during image decoding do not cause program crashes.*/

    png_init_io(png, file);
    png_set_sig_bytes(png, 8);

    png_read_info(png, info);

    // Get image dimensions and format
    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    png_byte colorType = png_get_color_type(png, info);
    png_byte bitDepth = png_get_bit_depth(png, info);

    // Ensure RGBA format
    if (bitDepth == 16)
        png_set_strip_16(png);
    if (colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (colorType != PNG_COLOR_TYPE_RGBA)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(png, info);

    // Read image data

    png_bytep* rowPointers = new png_bytep[height];
    png_size_t rowBytes = png_get_rowbytes(png, info);
    unsigned char* pixelData = new unsigned char[rowBytes * height];
    for (int y = 0; y < height; y++) {
        rowPointers[y] = pixelData + y * rowBytes;
    }
    png_read_image(png, rowPointers);

    // Cleanup libpng structures
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(file);
    delete[] rowPointers;

    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] pixelData;
}
