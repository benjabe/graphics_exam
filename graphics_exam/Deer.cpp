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
    m_position.x += delta_time;
    m_position.y = m_heightmap->map_height(m_position.x, m_position.z);
    //std::cout << glm::to_string(m_position) << '\n';
}

void Deer::render(const glm::mat4 & projection, const glm::mat4 & view, const glm::vec3 & view_position, const DirectionalLight & directional_light, const std::vector<PointLight>& point_lights)
{
    m_shader.use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    m_shader.set_mat4("model", model);
    m_shader.set_mat4("view", view);
    m_shader.set_mat4("projection", projection);
    m_model.draw(m_shader);
}
