#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "Cube.h"
#include "InputHandler.h"
#include "Heightmap.h"
#include "Light.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector> 
#include <random>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

class Game
{
private:
    int m_width;                // The width of the window
    int m_height;               // The height of the window
    GLFWwindow *m_window;       // The window the game runs in

    float m_last_time;          // Time at last frame

    bool m_first_mouse;         // Whether this is the first time the mouse moved
    glm::vec2 m_last_mouse_pos; // Mouse position in the previous frame

    Camera m_camera;            // The main (and likely only) camera
    
    std::vector<GameObject*> m_game_objects;

    glm::mat4 m_view;
    glm::mat4 m_projection;

    // Lights
    DirectionalLight m_directional_light;
    std::vector<PointLight> m_point_lights;
public:
    Game(unsigned int width, unsigned int height);
    ~Game();
    void start();
    void update(float delta_time);
    void render();
    void process_input(GLFWwindow *window, float delta_time);
    void add_point_light(glm::vec3 position);
};

