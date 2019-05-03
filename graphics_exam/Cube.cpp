#include "Cube.h"



Cube::Cube(const Shader &shader, const std::string &texture_path)
    : m_texture(Texture(texture_path))
{
    m_shader = shader;
    m_position = glm::vec3(0.0f);
    float vertices[] = {
        // positions          // colours 
        -0.5f, -0.5f, -0.5f,  0.46f, 0.31f, 0.22f,
         0.5f, -0.5f, -0.5f,  0.46f, 0.31f, 0.22f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   

        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  
        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
}


Cube::~Cube()
{
}

void Cube::update(float delta_time)
{
}

void Cube::render(const glm::mat4 &projection, const glm::mat4 &view)
{
    glBindVertexArray(m_vao);

    // Set up shader
    m_shader.use();
    m_shader.set_mat4("projection", projection);
    m_shader.set_mat4("view", view);
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    m_shader.set_mat4("model", model);
    m_shader.set_int("tex", m_texture.get_id());

    // Draw the cube
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
