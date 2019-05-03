#include "InputHandler.h"

std::map<std::string, int> InputHandler::m_keys = {};
std::map<int, bool> InputHandler::m_pressed = {};
std::map<int, bool> InputHandler::m_down = {};

glm::vec2 InputHandler::m_mouse_position = { 0.0f, 0.0f };
glm::vec2 InputHandler::m_mouse_offset = { 0.0f, 0.0f };

bool InputHandler::get_key(std::string key_name)
{
    return m_pressed[m_keys[key_name]];
}

bool InputHandler::get_key_down(std::string key_name)
{
    return m_down[m_keys[key_name]];
}

void InputHandler::add_key(std::string key_name, int key)
{
    m_keys[key_name] = key;
    m_pressed[key] = false;
    m_down[key] = false;
}

void InputHandler::set_key_pressed(int key)
{
    for (std::pair<int, bool> p : m_pressed)
    {
        if (p.first == key)
        {
            m_pressed[p.first] = true;
        }
        else
        {
            m_pressed[p.first] = false;
        }
    }
}

void InputHandler::set_key_down(int key)
{
    for (std::pair<int, bool> p : m_down)
    {
        if (p.first == key)
        {
            m_down[p.first] = true;
        }
        else
        {
            m_down[p.first] = false;
        }
    }
}

void InputHandler::update_mouse(double xpos, double ypos, double xoffset, double yoffset)
{
    m_mouse_position = {
        xpos,
        ypos
    };

    m_mouse_offset = {
        xoffset,
        yoffset
    };
}

glm::vec2 InputHandler::get_mouse_position()
{
    return m_mouse_position;
}

glm::vec2 InputHandler::get_mouse_offset()
{
    return m_mouse_offset;
}

void InputHandler::reset()
{
    for (std::pair<std::string, int> p : m_keys)
    {
        m_pressed[p.second] = false;
        m_down[p.second] = false;
    }
}

void InputHandler::update()
{
    for (std::pair<std::string, int> p : m_keys)
    {
        if (m_down[p.second])
        {
            std::cout << p.first << ": true\n";
        }
    }
}
