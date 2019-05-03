#pragma once

#include <glad/glad.h>
#include <stb_image.h>

#include <string>

class Texture
{
private:
    unsigned int m_id;
    std::string m_type;
    std::string m_path;
public:
    Texture(const std::string &path, const std::string &type = "null");
    ~Texture();
    unsigned int get_id();
    std::string get_type();
    std::string get_path();
    void render();
    void bind();
    void unbind();
};

