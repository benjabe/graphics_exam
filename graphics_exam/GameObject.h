#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include <vector>

class GameObject
{
protected:
    unsigned int m_vao;
    unsigned int m_shader;

    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;

public:
    GameObject();
    ~GameObject();
    virtual void update(float delta_time);
    virtual void render(const glm::mat4 &projection, const glm::mat4 &view);
};

