#pragma once

#include "GameObject.h"
#include "Raindrop.h"
#include "Game.h"

#include <random>

class Game;

class WeatherController :
    public GameObject
{
private:
    float m_spawn_rate;         // Raindrops per second
    float m_next_spawn_time;    // The time when the next raindrop should spawn
    float m_time;               // Time since instantiation
    float m_spawn_range;        // Raindrops spawn within this many units of m_position
public:
    WeatherController(glm::vec3 position);
    ~WeatherController();
    virtual void update(float delta_time);
    virtual void render(
        const glm::mat4 &projection,
        const glm::mat4 &view,
        const glm::vec3 &view_position,
        const DirectionalLight &directional_light,
        const std::vector<PointLight> &point_lights
    );
    void spawn();
};

