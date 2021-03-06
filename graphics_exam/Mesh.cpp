#include "Mesh.h"



Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    m_vertices = vertices;
    m_indices = indices;
    m_textures = textures;

    setup_mesh();
}

void Mesh::draw(Shader shader)
{
    unsigned int diffuse_nr = 1;
    unsigned int specular_nr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures[i].get_type();
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuse_nr++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specular_nr++);
        }

        shader.set_int((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].get_id());
    }
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::setup_mesh()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_vertices.size() * sizeof(MeshVertex),
        &m_vertices[0],
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_indices.size() * sizeof(unsigned int),
        &m_indices[0],
        GL_STATIC_DRAW
    );

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(MeshVertex),
        (void*)0
    );
    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(MeshVertex),
        (void*)offsetof(MeshVertex, normal)
    );
    // Vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(MeshVertex),
        (void*)offsetof(MeshVertex, tex_coords)
    );

    glBindVertexArray(0);
}
