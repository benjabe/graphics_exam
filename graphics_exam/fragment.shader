#version 330 core

struct Material
{
    float diffuse;
    float specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calc_dir_light(DirectionalLight light, vec3 normal, vec3 view_dir);

out vec4 frag_color;

in vec3 vertex_color;
in vec2 tex_coords;
in vec3 normal;
in vec3 frag_pos;

uniform vec3 view_pos;
uniform Material material;
uniform DirectionalLight directional_light;
uniform sampler2D tex;

void main()
{
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    frag_color = vec4(calc_dir_light(directional_light, norm, view_dir) + vertex_color, 1.0f);
    //frag_color = vec4(0.0f, (100.0f - frag_pos.g) / 120.0f, 0.0f, 1.0f); //texture(tex, tex_coords); //vec4(result, 1.0);
}

vec3 calc_dir_light(DirectionalLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.ambient;// *material.diffuse;
    vec3 diffuse = light.diffuse * diff;// *material.diffuse;
    vec3 specular = light.specular * spec;// *material.specular;
    
    return ambient + diffuse + specular;
}