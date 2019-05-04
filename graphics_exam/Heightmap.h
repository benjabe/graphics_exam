#pragma once

#include "GameObject.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <PerlinNoise.hpp>

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

const float HEIGHT_WATER = 0.0f;
const float HEIGHT_SHORE = 3.0f;
const float HEIGHT_PLAINS = 21.0f;
const float HEIGHT_MOUNTAIN = 64.0f;
const float HEIGHT_PEAK = 72.0f;

class Heightmap :
    public GameObject
{
private:
    int m_vertices;
    siv::PerlinNoise m_perlin;
    
    // for height generation
    double m_xoffset = 0.01;
    double m_yoffset = 0.013f;
    float m_min_height = -20.0f;
    float m_max_height = 90.0f;
public:
    Heightmap(const Shader &shader, int width = 20, int height = 20);
    Heightmap(const std::string & path);
    ~Heightmap();
    virtual void update(float delta_time);
    virtual void render(
        const glm::mat4 &projection,
        const glm::mat4 &view,
        const glm::vec3 &view_position,
        const DirectionalLight &directional_light,
        const std::vector<PointLight> &point_lights
    );
private:
    float map_height(double x, double y);
    glm::vec3 height_color(double x, double y);
};

