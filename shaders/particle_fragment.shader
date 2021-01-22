#version 450

in vec4 vColor;
in vec4 vPos;
in vec3 normal;
in vec2 tex_coord;
in mat3 TBN;

struct light
{
    vec3 position;
    vec3 color;
    vec3 dir;
    float intensity;
    int type;
    bool used;
};

uniform uint uniform_light;
uniform uint directional_light;
uniform uint ambient_light;

uniform light lights[8];

uniform sampler2D tex_sampler;
uniform sampler2D bump_map;

uniform vec4 color;

uniform int enableBumpMapping;

layout(location=0) out vec4 output_color;

void main() {
    vec3 surface_normal;
    if (enableBumpMapping == 1)
        // Convert normal map to world space
        surface_normal = normalize(TBN * normalize(texture2D(bump_map, tex_coord).rgb * 2.0 - 1));
    else
        surface_normal = normal;

    output_color = vec4(0);
    for (int i = 0; i < 8; i++)
    {
        vec4 light;
        if (lights[i].type == ambient_light)
            light = vec4(lights[i].color, 1.0);
        else if (lights[i].type == directional_light)
            light = vec4(lights[i].color, 1.0) * clamp(dot(surface_normal, lights[i].dir), 0, 1);
        else
        {
            vec4 light_beam = vec4(lights[i].position, 1.0);
            float dist = length(light_beam);
            vec3 dir = normalize(light_beam).xyz;
            float intensity = lights[i].intensity / (dist * dist);
            light = vec4(lights[i].color, 1.0) * clamp(dot(surface_normal, dir), 0, 1) * intensity;
        }

        output_color += texture2D(tex_sampler, tex_coord) * light;
    }
}


