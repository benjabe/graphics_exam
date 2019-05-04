#pragma once

#include "GameObject.h"
#include "Model.h"


class Raindrop :
    public GameObject
{
private:
    Model m_model;
    float m_speed = 0.0f;
    float m_gravity = -1.0f;
public:
    Raindrop();
    ~Raindrop();
    void update(float delta_time);
    virtual void render(
        const glm::mat4 &projection,
        const glm::mat4 &view,
        const glm::vec3 &view_position,
        const DirectionalLight &directional_light,
        const std::vector<PointLight> &point_lights
    );
};

