#pragma once

#include <glm/glm.hpp>

#include <map>
#include <string>
#include <iostream>

class InputHandler
{
private:
	static std::map<std::string, int> m_keys;
	static std::map<int, bool> m_pressed;
    static std::map<int, bool> m_down;

    static glm::vec2 m_mouse_position;
    static glm::vec2 m_mouse_offset;

public:
	//InputHandler();

public:
	static bool get_key(std::string key_name);
    static bool get_key_down(std::string key_name);
	static void add_key(std::string key_name, int key);
	static void set_key_pressed(int key);
    static void set_key_down(int key);
    static void update_mouse(double xpos, double ypos, double xoffset, double yoffset);
    static glm::vec2 get_mouse_position();
    static glm::vec2 get_mouse_offset();
    static void reset();
    static void update();
};
