#include "WeatherController.h"


WeatherController::WeatherController(glm::vec3 position)
{
    m_position = position;
    m_spawn_rate = 10.0f;
    m_next_spawn_time = 0.0f;
    m_spawn_range = 0.5f;
}


WeatherController::~WeatherController()
{
}

void WeatherController::update(float delta_time)
{
    m_time += delta_time;
    if (m_time >= m_next_spawn_time)
    {
        spawn();
        m_next_spawn_time = m_time + 1.0f / m_spawn_rate;
    }
}

void WeatherController::render(const glm::mat4 & projection, const glm::mat4 & view, const glm::vec3 & view_position, const DirectionalLight & directional_light, const std::vector<PointLight>& point_lights)
{
}

void WeatherController::spawn()
{
}
