#pragma once

#include "GameObject.h"
#include "Model.h"


class Raindrop :
    public GameObject
{
private:
    Model m_model;
public:
    Raindrop();
    ~Raindrop();
    virtual void render(
        const glm::mat4 &projection,
        const glm::mat4 &view,
        const glm::vec3 &view_position,
        const DirectionalLight &directional_light,
        const std::vector<PointLight> &point_lights
    );
};

