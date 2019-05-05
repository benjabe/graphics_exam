#include "Raindrop.h"



Raindrop::Raindrop(glm::vec3 position)
{
    m_shader = Shader("model_loading.vert", "raindrop.frag");
    m_shader.use();
    m_shader.set_vec3("color", { 0.2f, 0.2f, 0.8f });
    m_position = position;
    m_model = Model("Assets/Models/Raindrop/raindrop.obj");
}


Raindrop::~Raindrop()
{
}

void Raindrop::update(float delta_time)
{
    if (m_time < 5.0f)
    {
        m_time += delta_time;
        m_speed += m_gravity * delta_time;
        m_position.y += m_speed * delta_time;
    }
}

void Raindrop::render(const glm::mat4 & projection, const glm::mat4 & view, const glm::vec3 & view_position, const DirectionalLight & directional_light, const std::vector<PointLight>& point_lights)
{
    // Only render if lived less than 5 seconds
    if (m_time < 5.0f)
    {
        m_shader.use();
        glm::mat4 model(1.0f);
        model = glm::translate(model, m_position);
        m_shader.set_mat4("model", model);
        m_shader.set_mat4("view", view);
        m_shader.set_mat4("projection", projection);
        m_model.draw(m_shader);
    }
}
