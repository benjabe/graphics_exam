#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::update(float delta_time)
{
}

void GameObject::render(
    const glm::mat4 &projection,
    const glm::mat4 &view,
    const glm::vec3 &view_position,
    const DirectionalLight &directional_light,
    const std::vector<PointLight> &point_lights
)
{
}
