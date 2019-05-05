#include "Heightmap.h"

Heightmap::Heightmap(const Shader &shader, int width, int height)
{
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    m_position = glm::vec3(-1.0f);
    m_shader = shader;

    // Create vertices
    m_vertices = width * height * 6;
    Vertex *vertices = new Vertex[m_vertices];
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

        /* I should have studied music
        // Set up deer trail
        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &m_trail_texture);
        glBindTexture(GL_TEXTURE_2D, m_trail_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_trail_texture, 0);
        GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, draw_buffers);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::HEIGHTMAP::Framebuffer not complete: "
                      << glCheckFramebufferStatus(GL_FRAMEBUFFER) << '\n';
        }

        float quad_vertices[] = {
            // Pos     
            0.0f, 1.0f,
            1.0f, 0.0f, 
            0.0f, 0.0f, 

            0.0f, 1.0f, 
            1.0f, 1.0f, 
            1.0f, 0.0f
        };

        unsigned int vbo;
        unsigned int vao;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(quad_vertices),
            quad_vertices,
            GL_STATIC_DRAW
        );
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(float),
            (void*)0
        );

        Shader texture_drawing_shader("texture_drawing.vert", "texture_drawing.frag");
        texture_drawing_shader.use();
        glm::mat4 projection = glm::ortho(0, width, 0, height, -1, 1);
        texture_drawing_shader.set_mat4("projection", projection);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        const int PIXELS = 10;
        glm::ivec2 pixels[PIXELS] = {
            { 20, 20 },
            { 21, 20 },
            { 22, 21 },
            { 22, 22 },
            { 22, 23 },
            { 22, 24 },
            { 22, 25 },
            { 22, 26 },
            { 22, 27 },
            { 22, 29 }
        };
        for (int i = 0; i < PIXELS; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::scale(model, glm::vec3(50.0f));
            model = glm::translate(model, { pixels[i].x, pixels[i].y, 0 });
            texture_drawing_shader.set_mat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        */
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_vertices * sizeof(Vertex),
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

    m_shader.set_int("trail", m_trail_texture);

    // Draw the terrain 
    glDrawArrays(GL_TRIANGLES, 0, m_vertices);
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

void Heightmap::add_trail(glm::ivec2 position)
{
    std::stringstream string;
    int index = position.x * position.y;
    string << "trail[" << index << ']';
    m_shader.set_bool(string.str(), true);
}
