#pragma once

#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>

#include <string>
#include <vector>

struct MeshVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

class Mesh
{
private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;

    std::vector<MeshVertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

public:
    Mesh(
        std::vector<MeshVertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<Texture> textures);
    void draw(Shader shader);

private:
    void setup_mesh();
};

