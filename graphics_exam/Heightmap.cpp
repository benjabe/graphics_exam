#include "Heightmap.h"

Heightmap::Heightmap(const Shader &shader, int width, int height)
{
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    m_position = glm::vec3(-1.0f);
    m_shader = shader;

    // TODO: Generate heightmap

    // Create vertices
    m_indices = width * height * 6 * (3 + 3);
    float *vertices = new float[m_indices];
    int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            glm::vec3 color;
            // First triangle
            // Position
            vertices[index++] = x;
            vertices[index++] = map_height(x, y);
            vertices[index++] = y;
            // Color
            color = height_color(x, y);
            vertices[index++] = color.r;
            vertices[index++] = color.g;
            vertices[index++] = color.b;

            // Position
            vertices[index++] = x + 1.0f;
            vertices[index++] = map_height(x + 1.0f, y + 1.0f);
            vertices[index++] = y + 1.0f;
            // Color
            color = height_color(x + 1.0f, y + 1.0f);
            vertices[index++] = color.r;
            vertices[index++] = color.g;
            vertices[index++] = color.b;

            // Position
            vertices[index++] = x;
            vertices[index++] = map_height(x, y + 1.0f);
            vertices[index++] = y + 1.0f;
            // Color
            color = height_color(x, y + 1.0f);
            vertices[index++] = color.r;
            vertices[index++] = color.g;
            vertices[index++] = color.b;

            // Second triangle
            // Position
            vertices[index++] = x;
            vertices[index++] = map_height(x, y);
            vertices[index++] = y;
            // Color
            color = height_color(x, y);
            vertices[index++] = color.r;
            vertices[index++] = color.g;
            vertices[index++] = color.b;

            // Position
            vertices[index++] = x + 1.0f;
            vertices[index++] = map_height(x + 1.0f, y);
            vertices[index++] = y;
            // Color
            color = height_color(x + 1.0f, y);
            vertices[index++] = color.r;
            vertices[index++] = color.g;
            vertices[index++] = color.b;

            // Position
            vertices[index++] = x + 1.0f;
            vertices[index++] = map_height(x + 1.0f, y + 1.0f);
            vertices[index++] = y + 1.0f;
            // Color
            color = height_color(x + 1.0f, y + 1.0f);
            vertices[index++] = color.r;
            vertices[index++] = color.g;
            vertices[index++] = color.b;
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
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );
    // Color
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
    glDrawArrays(GL_TRIANGLES, 0, m_indices / 6);
}

float Heightmap::map_height(double x, double y)
{
    float noise = (m_perlin.noise(x * m_xoffset, y * m_yoffset) + 1.0f) / 2.0f;
    float range = m_max_height - m_min_height;
    float z = noise * range + m_min_height;
    return z;
}

glm::vec3 Heightmap::height_color(double x, double y)
{
    float z = map_height(x, y);
    float range = m_max_height - m_min_height;
    float percentage = z / range * 100.0f;
    if (percentage < HEIGHT_WATER)
    {
        return COLOR_WATER;
    }
    else if (percentage < HEIGHT_PLAINS)
    {
        return (COLOR_WATER + COLOR_PLAINS) / 2.0f;
    }
    else if (percentage < HEIGHT_MOUNTAIN)
    {
        return (COLOR_PLAINS + COLOR_MOUNTAIN) / 2.0f;
    }
    else
    {
        return (COLOR_MOUNTAIN + COLOR_PEAK) / 2.0f;
    }
}
