#version 450
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    TexCoords = aPos;
    gl_Position = projection_matrix * view_matrix * vec4(aPos, 1.0);
}  
