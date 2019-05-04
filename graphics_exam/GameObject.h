#pragma once

#include "Component.h"
#include "Light.h"
#include "Shader.h"

#include <glm/glm.hpp>

#include <vector>

class GameObject
{
protected:
    unsigned int m_vao;
    Shader m_shader;

    glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_scale;
    glm::vec3 m_rotation;

public:
    GameObject();
    ~GameObject();
    virtual void update(float delta_time);
    virtual void render(
        const glm::mat4 &projection,
        const glm::mat4 &view,
        const glm::vec3 &view_position,
        const DirectionalLight &directional_light,
        const std::vector<PointLight> &point_lights
    );
};

