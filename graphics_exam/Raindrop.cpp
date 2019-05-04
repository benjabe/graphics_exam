#include "Raindrop.h"



Raindrop::Raindrop()
{
    m_shader = Shader("model_loading.vert", "raindrop.frag");
    m_shader.use();
    m_shader.set_vec3("color", { 0.0f, 0.0f, 0.8f });
    m_position = glm::vec3(0.0f);
    m_model = Model("Assets/Models/Raindrop/raindrop.obj");
}


Raindrop::~Raindrop()
{
}

void Raindrop::render(const glm::mat4 & projection, const glm::mat4 & view, const glm::vec3 & view_position, const DirectionalLight & directional_light, const std::vector<PointLight>& point_lights)
{
    // Set up shader
    m_shader.use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    m_shader.set_mat4("model", model);
    m_shader.set_mat4("view", view);
    m_shader.set_mat4("projection", projection);
    m_model.draw(m_shader);
}