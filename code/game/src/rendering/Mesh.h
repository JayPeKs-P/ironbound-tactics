//
// Created by julia on 23/10/2024.
//

#pragma once

#include <map>
#include <string>
#include <__msvc_filebuf.hpp>

#include <glm/glm.hpp>
#include <tiny_gltf/tiny_gltf.h>


namespace gl3 {

class Mesh {
  public:
    explicit Mesh(const std::filesystem::path &gltfAssetPath, int meshIndex = 0);
    ~Mesh();
    void draw() const;

    // Delete copy constructor
    Mesh(const Mesh &other) = delete;

    // Explicit move constructor
    Mesh(Mesh &&other) noexcept {
        //new Code day 4
        std::swap(this->VAO, other.VAO);
        std::swap(this->buffers, other.buffers);
        std::swap(this->vaa, other.vaa);
        std::swap(this->primitives, other.primitives);
    }
    struct Primitive {
        int mode;
        size_t count;
        int type;
    };
private:
    unsigned int VAO = 0;
    std::map<int, unsigned int> buffers;
    std::map<std::string, unsigned int> vaa = {
        {"POSITION", 0},
        {"NORMAL", 1}
    };
    std::vector<Primitive> primitives;
    tinygltf::Model Mesh::loadGltf(const std::filesystem::path &gltfAssetPath);


};

} // gl3