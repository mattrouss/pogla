#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 uv_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in mat4 transform_a;
layout(location = 8) in vec3 velocity_a;
layout(location = 9) in float angle_a;
layout(location = 10) in mat4 transform_b;
layout(location = 14) in vec3 velocity_b;
layout(location = 15) in float angle_b;

uniform vec4 color;
uniform mat4 view_matrix;// = mat4(
			      //0, 0, 1, 0,
			      //0, 1, 0, 0,
			      //1, 0, 0, 0,
			      //0, 0, -10, 1);
uniform mat4 projection_matrix; // = mat4(
			      //5.00000, 0.00000, 0.00000, 0.00000,
			      //0.00000, 5.00000, 0.00000, 0.00000,
			      //0.00000, 0.00000, -1.00020, -1.00000,
			      //0.00000, 0.00000, -10.00100, 0.00000);

uniform int enableBumpMapping;

out vec4 vColor;
out vec4 vPos;
out vec3 normal;
out vec2 tex_coord;
out mat3 TBN;

mat4 look_at(vec3 forward)
{
    vec3 f = normalize(forward);
    vec3 l = normalize(cross(vec3(0, 1, 0), f));
    vec3 u = cross(f, l);

    return mat4(
         l[0],  l[1],  l[2], 0,
         u[0],  u[1],  u[2], 0,
        f[0], f[1], f[2], 0,
            0,     0,     0, 1
        );

}

void main() {
    mat4 rot = look_at(velocity_a);
    gl_Position = projection_matrix * view_matrix * transform_a * rot * vec4(position, 1.0);
    vColor = color;
    normal = (rot * vec4(normals, 1.0)).xyz;

    if (enableBumpMapping == 1) {
        vec3 bitangent = normalize(cross(tangent, normal));
        // Compute matrix to convert vectors from tangent to world space
        TBN =
            mat3(normalize(vec3(transform_a * vec4(tangent, 0.0))),
                 normalize(vec3(transform_a * vec4(-bitangent, 0.0))),
                 normalize(vec3(transform_a * vec4(normal, 0.0))));
    }

    tex_coord = uv_coord;
    vPos = vec4(position, 1.0);
}


