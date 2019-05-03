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

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
};

const glm::vec3 COLOR_PEAK = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 COLOR_SHORE = glm::vec3(0.52f, 0.48f, 0.0f);
const glm::vec3 COLOR_MOUNTAIN = glm::vec3(0.46f, 0.31f, 0.22f);
const glm::vec3 COLOR_PLAINS = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 COLOR_WATER = glm::vec3(0.0f, 0.0f, 1.0f);

const float HEIGHT_WATER = 25.0f;
const float HEIGHT_SHORE = 27.0f;
const float HEIGHT_PLAINS = 42.0f;
const float HEIGHT_MOUNTAIN = 70.0f;
const float HEIGHT_PEAK = 90.0f;

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
    float map_height(double x, double y);
    glm::vec3 height_color(double x, double y);
};

