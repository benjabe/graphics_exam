#include "Heightmap.h"

Heightmap::Heightmap(const Shader &shader, int width, int height)
{
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    m_position = glm::vec3(-1.0f);
    m_shader = shader;

    // TODO: Generate heightmap

    // Create vertices
    m_indices = width * height * 6 * 3;
    float *vertices = new float[m_indices];
    int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            std::cout << x << ", " << y << ":\n";

            float lower_left = dis(gen);
            float upper_right = dis(gen);

            // First triangle
            vertices[index++] = x;
            vertices[index++] = lower_left;
            vertices[index++] = y;

            vertices[index++] = x + 1.0f;
            vertices[index++] = upper_right;
            vertices[index++] = y + 1.0f;

            vertices[index++] = x;
            vertices[index++] = dis(gen);
            vertices[index++] = y + 1.0f;

            std::cout << "\tFirst triangle:\n";
            for (int i = 0; i < 3; i++)
            {
                std::cout << "\t\t" << vertices[index - 9 + 3 * i]
                    << ", " << vertices[index - 9 + 3 * i + 1]
                    << ", " << vertices[index - 9 + 3 * i + 2] << '\n';
            }

            // Second triangle
            vertices[index++] = x;
            vertices[index++] = lower_left;
            vertices[index++] = y;

            vertices[index++] = x + 1.0f;
            vertices[index++] = dis(gen);
            vertices[index++] = y;

            vertices[index++] = x + 1.0f;
            vertices[index++] = upper_right;
            vertices[index++] = y + 1.0f;

            std::cout << "\tSecond triangle:\n";
            for (int i = 0; i < 3; i++)
            {
                std::cout << "\t\t" << vertices[index - 9 + 3 * i]
                    << ", " << vertices[index - 9 + 3 * i + 1]
                    << ", " << vertices[index - 9 + 3 * i + 2] << '\n';
            }
        }
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_indices * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    /*
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );
    */
}

Heightmap::Heightmap(const std::string &path)
{
}


Heightmap::~Heightmap()
{
}

void Heightmap::update(float delta_time)
{
}

void Heightmap::render(const glm::mat4 &projection, const glm::mat4 &view)
{
    glBindVertexArray(m_vao);

    // Set up shader
    m_shader.use();
    m_shader.set_mat4("projection", projection);
    m_shader.set_mat4("view", view);
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    m_shader.set_mat4("model", model);

    // Draw the terrain 
    glDrawArrays(GL_TRIANGLES, 0, m_indices / 3);
}
