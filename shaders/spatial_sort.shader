#version 450

const uint PARTICLE_NB = 5;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
const float period = 2.0;

struct Particle
{
    float pos_x, pos_y, pos_z;
};

layout (std430, binding=1) buffer particle_pos_buffer
{
    Particle particles[];
};

void swap_horizontal(int i_a, int i_b)
{
    if (particles[i_a].pos_x > particles[i_b].pos_x)
    {
        float x = particles[i_a].pos_x;
        float y = particles[i_a].pos_y;
        float z = particles[i_a].pos_z;
        particles[i_a].pos_x = particles[i_b].pos_x;
        particles[i_a].pos_y = particles[i_b].pos_y;
        particles[i_a].pos_z = particles[i_b].pos_z;
        particles[i_b].pos_x = x;
        particles[i_b].pos_y = y;
        particles[i_b].pos_z = z;
    }
}

void swap_vertical(int i_a, int i_b)
{
    if (particles[i_a].pos_y > particles[i_b].pos_y)
    {
        float x = particles[i_a].pos_x;
        float y = particles[i_a].pos_y;
        float z = particles[i_a].pos_z;
        particles[i_a].pos_x = particles[i_b].pos_x;
        particles[i_a].pos_y = particles[i_b].pos_y;
        particles[i_a].pos_z = particles[i_b].pos_z;
        particles[i_b].pos_x = x;
        particles[i_b].pos_y = y;
        particles[i_b].pos_z = z;
    }
}

void main()
{
    uint i_a = gl_GlobalInvocationID.x;
    uint i_b = i_a + 1;
    if (gl_LocalInvocationID.x % 2 == 0
        && gl_LocalInvocationID.x < gl_WorkGroupSize.x - 1)
    {
        swap_horizontal(i_a, i_b);
    }
    barrier();
    if (gl_LocalInvocationID.x % 2 != 0
        && gl_LocalInvocationID.x < gl_WorkGroupSize.x - 1)
    {
        swap_horizontal(i_a, i_b);
    }
    barrier();
    i_b = i_a + gl_WorkGroupSize.x * gl_NumWorkGroups.x;
    if (gl_LocalInvocationID.y % 2 == 0
        && gl_LocalInvocationID.y < gl_WorkGroupSize.y - 1)
    {
        swap_vertical(i_a, i_b);
    }
    barrier();
    if (gl_LocalInvocationID.y % 2 != 0
        && gl_LocalInvocationID.y < gl_WorkGroupSize.y - 1)
    {
        swap_vertical(i_a, i_b);
    }
}