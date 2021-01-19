#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 uv_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 offset_a;
layout(location = 5) in float orientation_angle;
layout(location = 6) in vec3 offset_b;
uniform vec4 color;
uniform mat4 model_matrix;
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

mat4 rotationY(float angle) {
    return mat4(   cos(angle),		0,		sin(angle),	0,
                            0,	  1.0,			     0,	0,
                  -sin(angle),	    0,		cos(angle),	0,
                            0, 		0,				 0,	1);
}

void main() {
    mat4 oriented_model_matrix = rotationY(orientation_angle) * model_matrix;
    gl_Position = projection_matrix * view_matrix * oriented_model_matrix * vec4(position + offset_b, 1.0);
    vColor = color;
    normal = normals;

    if (enableBumpMapping == 1) {
        vec3 bitangent = normalize(cross(tangent, normal));
        // Compute matrix to convert vectors from tangent to world space
        TBN =
            mat3(normalize(vec3(model_matrix * vec4(tangent, 0.0))),
                normalize(vec3(model_matrix * vec4(-bitangent, 0.0))),
                normalize(vec3(model_matrix * vec4(normal, 0.0))));
    }

    tex_coord = uv_coord;
    vPos = vec4(position, 1.0);
}


