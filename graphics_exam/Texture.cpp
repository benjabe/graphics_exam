#include "Texture.h"



Texture::Texture(const std::string &path, const std::string &type)
{
    m_path = path.substr(1, path.find_last_of('/'));
    m_type = type;

    glGenTextures(1, &m_id);

    int width;
    int height;
    int nr_components;
    unsigned char *data = stbi_load(
        path.c_str(),
        &width,
        &height,
        &nr_components,
        0
    );

    if (data)
    {
        GLenum format;
        switch (nr_components)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(
            GL_TEXTURE_2D, 
            GL_TEXTURE_WRAP_S, 
            GL_REPEAT
        );
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_REPEAT
        );
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR
        );
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );
    }

    stbi_image_free(data);
}


Texture::~Texture()
{
}

unsigned int Texture::get_id()
{
    return m_id;
}

std::string Texture::get_type()
{
    return m_type;
}

std::string Texture::get_path()
{
    return m_path;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
