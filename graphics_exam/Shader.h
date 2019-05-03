#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
private:
    unsigned int m_id;
public:
    Shader();
    Shader(const char *vertex_path, const char *fragment_path);
    ~Shader();
    void use();
    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    void set_mat4(const std::string &name, glm::mat4 value) const;
    void set_vec3(const std::string &name, glm::vec3 value) const;

    unsigned int get_id() const;
};

