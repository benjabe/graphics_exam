#version 330 core
layout (location = 0) out vec3 color;

in vec3 vertex_color;

uniform sampler2D tex;

void main()
{
    color = vertex_color;
}