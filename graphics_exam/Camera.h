#pragma once

#include "InputHandler.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

enum Direction
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;

    float m_yaw;
    float m_pitch;
    float m_roll;

    float m_movement_speed;
    float m_sensitivity;
    float m_zoom;
public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = 0.0f,
        float pitch = 0.0f
    );
    void process_keyboard(Direction direction, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset);

    void update(float delta_time);

    glm::mat4 get_view_matrix();
    glm::vec3 get_position();
private:
    void update_camera_vectors();
};

