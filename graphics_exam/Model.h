#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include <vector>

unsigned int load_texture(
    const char *path,
    const std::string &directory);

class Model
{
private:
    std::vector<Texture> m_textures_loaded;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    bool m_gamma_correction;

 public:
    Model(const char *path);
    void draw(Shader shader);

private:
    void load_model(const std::string &path);
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> load_material_textures(
        aiMaterial *mat,
        aiTextureType type,
        std::string type_name);
};

