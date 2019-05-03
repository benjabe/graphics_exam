#include "Shader.h"

Shader::Shader()
{
}

Shader::Shader(const char *vertex_path, const char *fragment_path)
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit |
        std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit |
        std::ifstream::badbit);

    try
    {
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);
        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Failed to read file\n";
    }
    const char *vertex_shader_code = vertex_code.c_str();
    const char *fragment_shader_code = fragment_code.c_str();

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, info_log);
        std::cout << "vertex shader:\n" << info_log << '\n';
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, info_log);
        std::cout << "fragment shader:\n" << info_log << '\n';
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, nullptr, info_log);
        std::cout << "Shader program:\n" << info_log << '\n';
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


Shader::~Shader()
{
}

void Shader::use()
{
    glUseProgram(m_id);
}

void Shader::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_mat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(
        glGetUniformLocation(m_id, name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}

void Shader::set_vec3(const std::string &name, glm::vec3 value) const
{
    glUniform3fv(
        glGetUniformLocation(m_id, name.c_str()),
        1,
        glm::value_ptr(value)
    );
}

unsigned int Shader::get_id() const
{
    return m_id;
}