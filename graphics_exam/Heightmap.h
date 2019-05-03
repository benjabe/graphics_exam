#pragma once

#include "GameObject.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <PerlinNoise.hpp>

#include <iostream>
#include <vector>
#include <random>



class Heightmap :
    public GameObject
{
private:
    Shader m_shader;
    int m_indices;
    siv::PerlinNoise m_perlin;
    
    // for height generation
    double m_xoffset = 0.005;
    double m_yoffset = 0.009f;
    float m_min_height = 0.0f;
    float m_max_height = 100.0f;
public:
    Heightmap(const Shader &shader, int width = 20, int height = 20);
    Heightmap(const std::string & path);
    ~Heightmap();
    virtual void update(float delta_time);
    void render(const glm::mat4 &projection, const glm::mat4 &view);
private:
    float noise(double x, double y);
};

