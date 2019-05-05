#include "Deer.h"



Deer::Deer(Heightmap *heightmap, glm::vec3 position)
{
    m_heightmap = heightmap;
    m_position = position;
    m_shader = Shader("model_loading.vert", "model_loading.frag");
    m_model = Model("Assets/Models/Deer/deer.obj");
}


Deer::~Deer()
{
}

void Deer::update(float delta_time)
{
    //std::cout << glm::to_string(m_position) << '\n';
}

void Deer::process_keyboard(Direction direction, float delta_time, glm::vec3 front, glm::vec3 right)
{
    front.y = 0;
    right.y = 0;
    float speed = delta_time * 20.4f;
    switch (direction)
    {
    case FORWARD:
        m_position += front * speed;
        break;
    case BACKWARD:
        m_position -= front * speed;
        break;
    case LEFT:
        m_position -= right * speed;
        break;
    case RIGHT:
        m_position += right * speed;
        break;
    default:
        break;
    }
    m_position.y = m_heightmap->map_height(m_position.x, m_position.z);
}

glm::vec3 Deer::get_position()
{
    return m_position;
}

void Deer::render(const glm::mat4 & projection, const glm::mat4 & view, const glm::vec3 & view_position, const DirectionalLight & directional_light, const std::vector<PointLight>& point_lights)
{
    m_shader.use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    m_shader.set_mat4("model", model);
    m_shader.set_mat4("view", view);
    m_shader.set_mat4("projection", projection);
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
    m_model.draw(m_shader);
}
