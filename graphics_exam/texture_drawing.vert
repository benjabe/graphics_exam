#version 330 core
layout (location = 0) in vec2 a_pos;

uniform mat4 model;
uniform mat4 projection;

out vec3 vertex_color;

void main()
{
    gl_Position = projection * model * vec4(a_pos, 0.0, 1.0);
    vertex_color = vec3(1.0, 1.0, 1.0);
}