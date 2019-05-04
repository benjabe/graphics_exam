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

            // Normal
            glm::vec3 u = vertices[index - 2].position - vertices[index - 3].position;
            glm::vec3 v = vertices[index - 1].position - vertices[index - 3].position;
            glm::vec3 normal{
                u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x
            };
            // apply normal
            for (int i = 1; i < 4; i++)
            {
                vertices[index - i].normal = normal;
            }

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

            // Normal (should be a lambda or something but but)
            u = vertices[index - 2].position - vertices[index - 3].position;
            v = vertices[index - 1].position - vertices[index - 3].position;
            normal = {
                u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x
            };
            // apply normal
            for (int i = 1; i < 4; i++)
            {
                vertices[index - i].normal = normal;
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

void Heightmap::render(
    const glm::mat4 &projection,
    const glm::mat4 &view,
    const glm::vec3 &view_position,
    const DirectionalLight &directional_light,
    const std::vector<PointLight> &point_lights)
{
    glBindVertexArray(m_vao);

    // Set up shader
    m_shader.use();
    m_shader.set_mat4("projection", projection);
    m_shader.set_mat4("view", view);
    m_shader.set_vec3("view_pos", view_position);
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    m_shader.set_mat4("model", model);
    m_shader.set_vec3(
        "directional_light.direction",
        directional_light.direction
    );
    m_shader.set_vec3(
        "directional_light.ambient",
        directional_light.ambient
    );
    m_shader.set_vec3(
        "directional_light.diffuse",
        directional_light.diffuse
    );
    m_shader.set_vec3(
        "directional_light.specular",
        directional_light.specular
    );
    m_shader.set_float("material.diffuse", 0.2f);
    m_shader.set_float("material.specular", 0.1f);
    m_shader.set_float("material.shininess", 512.0f);

    m_shader.set_int("nr_point_lights", point_lights.size());
    for (int i = 0; i < point_lights.size(); i++)
    {
        std::stringstream string;
        string << "point_lights[" << i << "].position";
        m_shader.set_vec3(string.str(), point_lights[i].position);
        string.str(std::string());

        string << "point_lights[" << i << "].ambient";
        m_shader.set_vec3(string.str(), point_lights[i].ambient);
        string.str(std::string());

        string << "point_lights[" << i << "].diffuse";
        m_shader.set_vec3(string.str(), point_lights[i].diffuse);
        string.clear();
        string.str(std::string());

        string << "point_lights[" << i << "].specular";
        m_shader.set_vec3(string.str(), point_lights[i].specular);
        string.str(std::string());

        string << "point_lights[" << i << "].constant";
        m_shader.set_float(string.str(), point_lights[i].constant);
        string.str(std::string());

        string << "point_lights[" << i << "].linear";
        m_shader.set_float(string.str(), point_lights[i].linear);
        string.str(std::string());

        string << "point_lights[" << i << "].quadratic";
        m_shader.set_float(string.str(), point_lights[i].quadratic);
        string.str(std::string());
    }

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
    }
    if (noise > max)
    {
        max = noise;
    }
    return z;
}

glm::vec3 Heightmap::height_color(double x, double y)
{
    float z = map_height(x, y);
    float range = m_max_height - m_min_height;
    float percentage = z / range * 100.0f;
    float interpolation_value = 0.0f;
    glm::vec3 result(0.0f);
    if (z < HEIGHT_WATER)
    {
        result = COLOR_WATER;
    }
    else if (z < HEIGHT_SHORE)
    {
        // TODO: Switch away from linear interpolation (?)
        interpolation_value = (z - HEIGHT_WATER) / (HEIGHT_SHORE - HEIGHT_WATER);
        result = (1.0f - interpolation_value) * COLOR_WATER + interpolation_value * COLOR_SHORE;
    }
    else if (z < HEIGHT_PLAINS)
    {
        interpolation_value = (z - HEIGHT_SHORE) / (HEIGHT_PLAINS - HEIGHT_SHORE);
        result = (1.0f - interpolation_value) * COLOR_SHORE + interpolation_value * COLOR_PLAINS;
    }
    else if (z < HEIGHT_MOUNTAIN)
    {
        interpolation_value = (z - HEIGHT_PLAINS) / (HEIGHT_MOUNTAIN - HEIGHT_PLAINS);
        result = (1.0f - interpolation_value) * COLOR_PLAINS + interpolation_value * COLOR_MOUNTAIN;
    }
    else if (z < HEIGHT_PEAK)
    {
        interpolation_value = (z - HEIGHT_MOUNTAIN) / (HEIGHT_PEAK - HEIGHT_MOUNTAIN);
        result = (1.0f - interpolation_value) * COLOR_MOUNTAIN + interpolation_value * COLOR_PEAK;
    }
    else
    {
        result = COLOR_PEAK;
    }
    return result;
}
