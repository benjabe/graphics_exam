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

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
#define MAX_POINT_LIGHTS 10

vec3 calc_dir_light(DirectionalLight light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

out vec4 frag_color;

in vec3 vertex_color;
in vec3 normal;
in vec3 frag_pos;

uniform vec3 view_pos;
uniform Material material;
uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int nr_point_lights;

void main()
{
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    // Light
    vec3 result = calc_dir_light(directional_light, norm, view_dir);
    for (int i = 0; i < nr_point_lights && i < MAX_POINT_LIGHTS; i++)
    {
        result += calc_point_light(point_lights[i], norm, frag_pos, view_dir);
    }
    result *= vertex_color;
    frag_color = vec4(result, 1.0);
}

vec3 calc_dir_light(DirectionalLight light, vec3 normal, vec3 view_dir)
{
    //normal = vec3(0.0f, 1.0f, 0.0f); // Debugging
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

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);

    // Diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance
        + light.quadratic * (distance * distance));

    // Combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    return (ambient + diffuse + specular) * attenuation;
}