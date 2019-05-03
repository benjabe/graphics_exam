#include "Heightmap.h"

Heightmap::Heightmap(const Shader &shader, int width, int height)
{
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    m_position = glm::vec3(-1.0f);
    m_shader = shader;

    // TODO: Generate heightmap

    // Create vertices
    m_indices = width * height * 6;
    Vertex *vertices = new Vertex[m_indices];
    int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Triangle 1
            vertices[index++] = {
                { x, map_height(x, y), y }, // Position
                height_color(x, y),         // Colour
                glm::vec3(1.0f)             // Normal
            };
            vertices[index++] = {
                { x + 1.0f, map_height(x + 1.0f, y + 1.0f), y + 1.0f },
                height_color(x + 1.0f, y + 1.0f),
                glm::vec3(1.0f)
            };
            vertices[index++] = {
                { x, map_height(x, y + 1.0f), y + 1.0f },
                height_color(x, y + 1.0f),
                glm::vec3(1.0f)
            };

            // Triangle 2
            vertices[index++] = {
                { x, map_height(x, y), y }, // Position
                height_color(x, y),         // Colour
                glm::vec3(1.0f)             // Normal
            };

            vertices[index++] = {
                { x + 1.0f, map_height(x + 1.0f, y), y },
                height_color(x + 1.0f, y),
                glm::vec3(1.0f)
            };

            vertices[index++] = {
                { x + 1.0f, map_height(x + 1.0f, y + 1.0f), y + 1.0f },
                height_color(x + 1.0f, y + 1.0f),
                glm::vec3(1.0f)
            };
        }
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_indices * sizeof(Vertex),
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
        sizeof(Vertex),
        (void*)0
    );
    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)(offsetof(Vertex, color))
    );
    // Normal 
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)(offsetof(Vertex, normal))
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
    glDrawArrays(GL_TRIANGLES, 0, m_indices);
}

float Heightmap::map_height(double x, double y)
{

    static float max = -9999;
    static float min = 9999;

    float noise = (m_perlin.noise(x * m_xoffset, y * m_yoffset) + 1.0f) / 2.0f;
    float range = m_max_height - m_min_height;
    float z = noise * range + m_min_height;
    if (noise < min)
    {
        min = noise;
        std::cout << "MIN: " << min << '\n';
    }
    if (noise > max)
    {
        max = noise;
        std::cout << "MAX: " << max << '\n';
    }
    return z;
}

glm::vec3 Heightmap::height_color(double x, double y)
{
    float z = map_height(x, y) - m_min_height;
    float range = m_max_height - m_min_height;
    float percentage = z / range * 100.0f;
    float interpolation_value = 0.0f;
    glm::vec3 result(0.0f);
    if (percentage < HEIGHT_WATER)
    {
        result = COLOR_WATER;
    }
    else if (percentage < HEIGHT_SHORE)
    {
        // TODO: Switch away from linear interpolation (?)
        interpolation_value = (percentage - HEIGHT_WATER) / (HEIGHT_SHORE - HEIGHT_WATER);
        result = (1.0f - interpolation_value) * COLOR_WATER + interpolation_value * COLOR_SHORE;
    }
    else if (percentage < HEIGHT_PLAINS)
    {
        interpolation_value = (percentage - HEIGHT_SHORE) / (HEIGHT_PLAINS - HEIGHT_SHORE);
        result = (1.0f - interpolation_value) * COLOR_SHORE + interpolation_value * COLOR_PLAINS;
    }
    else if (percentage < HEIGHT_MOUNTAIN)
    {
        interpolation_value = (percentage - HEIGHT_PLAINS) / (HEIGHT_MOUNTAIN - HEIGHT_PLAINS);
        result = (1.0f - interpolation_value) * COLOR_PLAINS + interpolation_value * COLOR_MOUNTAIN;
    }
    else if (percentage < HEIGHT_PEAK)
    {
        interpolation_value = (percentage - HEIGHT_MOUNTAIN) / (HEIGHT_PEAK - HEIGHT_MOUNTAIN);
        result = (1.0f - interpolation_value) * COLOR_MOUNTAIN + interpolation_value * COLOR_PEAK;
    }
    else
    {
        result = COLOR_PEAK;
    }
    return result;
}
