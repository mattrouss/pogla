#version 450

const uint PARTICLE_NB = 1;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
uniform uint parity;
const float period = 2.0;

struct Particle
{
    float pos_x, pos_y, pos_z;
    float vel_x, vel_y, vel_z;
};

layout (std430, binding=1) buffer particle_pos_buffer_a
{
    Particle particles_a[];
};

layout (std430, binding=2) buffer particle_pos_buffer_b
{
    Particle particles_b[];
};

void swap_horizontal(uint i_a, uint i_b)
{
    Particle tmp;
    if (parity == 0)
    {
        if (particles_a[i_a].pos_x > particles_a[i_b].pos_x)
        {
            tmp = particles_a[i_a];
            particles_a[i_a].pos_x = particles_a[i_b].pos_x;
            particles_a[i_a].pos_y = particles_a[i_b].pos_y;
            particles_a[i_a].pos_z = particles_a[i_b].pos_z;
            particles_a[i_b].pos_x = tmp.pos_x;
            particles_a[i_b].pos_y = tmp.pos_y;
            particles_a[i_b].pos_z = tmp.pos_z;
        }
    } else {
        if (particles_b[i_a].pos_x > particles_b[i_b].pos_x)
        {
            tmp = particles_b[i_a];
            particles_b[i_a].pos_x = particles_b[i_b].pos_x;
            particles_b[i_a].pos_y = particles_b[i_b].pos_y;
            particles_b[i_a].pos_z = particles_b[i_b].pos_z;
            particles_b[i_b].pos_x = tmp.pos_x;
            particles_b[i_b].pos_y = tmp.pos_y;
            particles_b[i_b].pos_z = tmp.pos_z;
        }
    }
}

void swap_vertical(uint i_a, uint i_b)
{
    Particle tmp;
    if (parity == 0)
    {
        if (particles_a[i_a].pos_z > particles_a[i_b].pos_z)
        {
            tmp = particles_a[i_a];
            particles_a[i_a].pos_x = particles_a[i_b].pos_x;
            particles_a[i_a].pos_y = particles_a[i_b].pos_y;
            particles_a[i_a].pos_z = particles_a[i_b].pos_z;
            particles_a[i_b].pos_x = tmp.pos_x;
            particles_a[i_b].pos_y = tmp.pos_y;
            particles_a[i_b].pos_z = tmp.pos_z;
        }
    } else {
        if (particles_b[i_a].pos_z > particles_b[i_b].pos_z)
        {
            tmp = particles_b[i_a];
            particles_b[i_a].pos_x = particles_b[i_b].pos_x;
            particles_b[i_a].pos_y = particles_b[i_b].pos_y;
            particles_b[i_a].pos_z = particles_b[i_b].pos_z;
            particles_b[i_b].pos_x = tmp.pos_x;
            particles_b[i_b].pos_y = tmp.pos_y;
            particles_b[i_b].pos_z = tmp.pos_z;
        }
    }
}

void main()
{
    uint i_a = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_WorkGroupSize.x * gl_NumWorkGroups.x;
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
