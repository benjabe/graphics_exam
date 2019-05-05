#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_movement_speed = 42.0f;
    m_sensitivity = 1.0f;
    m_zoom = 90.0f;

    m_position = position;
    m_world_up = up;
    m_yaw = yaw;
    m_pitch = pitch;

    update_camera_vectors();
}

void Camera::process_keyboard(Direction direction, float delta_time)
{
    float speed = m_movement_speed * delta_time;
    switch (direction)
    {
    case FORWARD:
        m_position += m_front * speed;
        break;
    case BACKWARD:
        m_position -= m_front * speed;
        break;
    case LEFT:
        m_position -= m_right * speed;
        break;
    case RIGHT:
        m_position += m_right * speed;
        break;
    default:
        break;
    }
}

void Camera::process_mouse_movement(float x_offset, float y_offset)
{
    x_offset *= m_sensitivity;
    y_offset *= m_sensitivity;

    m_yaw += x_offset;
    m_pitch += y_offset;
    if (m_pitch > 89.5f)
    {
        m_pitch = 89.5f;
    }
    if (m_pitch < -89.5f)
    {
        m_pitch = -89.5f;
    }

    update_camera_vectors();
}

void Camera::update(float delta_time)
{
    float speed = m_movement_speed * delta_time;
    if (InputHandler::get_key("up"))
    {
        m_position += m_front * speed;
    }
    if (InputHandler::get_key("down"))
    {
        m_position -= m_front * speed;
    }
    if (InputHandler::get_key("left"))
    {
        m_position -= m_right * speed;
    }
    if (InputHandler::get_key("right"))
    {
        m_position += m_right * speed;
    }

    m_yaw += InputHandler::get_mouse_offset().x;
    m_yaw += InputHandler::get_mouse_offset().y;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::get_position()
{
    return m_position;
}

glm::vec3 Camera::get_front()
{
    return m_front;
}

glm::vec3 Camera::get_right()
{
    return m_right;
}

void Camera::set_position(glm::vec3 position)
{
    m_position = position;
}

void Camera::update_camera_vectors()
{
    m_front = glm::normalize(glm::vec3(
        cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        sin(glm::radians(m_pitch)),
        sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
    ));
    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
