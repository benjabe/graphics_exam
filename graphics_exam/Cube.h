#pragma once

#include "GameObject.h"
#include "Texture.h"
#include "Shader.h"

class Cube :
    public GameObject
{
private:
    Texture m_texture;
    Shader m_shader;

public:
    Cube(const Shader &shader, const std::string &texture_path);
    ~Cube();
    virtual void update(float delta_time);
    virtual void render(const glm::mat4 &projection, const glm::mat4 &view);
};

