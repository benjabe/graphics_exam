#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 frag_color;

in vec2 tex_coords;
in vec3 normal;
in vec3 frag_pos;

uniform vec3 view_pos;
uniform Material material;
uniform Light light;
uniform sampler2D tex;

void main()
{
    // ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));

    // diffuse light
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));

    // specular light
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflection_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflection_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));

    // final colour
    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(0.8f, 9.0f, 0.2f, 1.0f); //texture(tex, tex_coords); //vec4(result, 1.0);
}