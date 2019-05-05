#pragma once

#include "GameObject.h"
#include "Model.h"
#include "Heightmap.h"
#include "Camera.h"


class Deer :
    public GameObject
{
private:
    Model m_model;
    Heightmap *m_heightmap;

public:
    Deer(Heightmap *heightmap, glm::vec3 position);
    ~Deer();
    virtual void update(float delta_time);
    virtual void render(
        const glm::mat4 &projection,
        const glm::mat4 &view,
        const glm::vec3 &view_position,
        const DirectionalLight &directional_light,
        const std::vector<PointLight> &point_lights
    );
    void process_keyboard(Direction direction, float delta_time, glm::vec3 front, glm::vec3 right);
    glm::vec3 get_position();
};

