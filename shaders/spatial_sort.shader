#version 450

const uint PARTICLE_NB = 1;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
uniform uint parity;
uniform uint sortStep;
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
        if (particles_a[i_a].pos_x > particles_a[i_b].pos_x
            || (particles_a[i_a].pos_x == particles_a[i_b].pos_x
                && particles_a[i_a].pos_z > particles_a[i_b].pos_z))
        {
            tmp = particles_a[i_a];
            particles_a[i_a] = particles_a[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_a[i_b] = tmp;
        }
    } else {
        if (particles_b[i_a].pos_x > particles_b[i_b].pos_x
            || (particles_b[i_a].pos_x == particles_b[i_b].pos_x
                && particles_b[i_a].pos_z > particles_b[i_b].pos_z))
        {
            tmp = particles_b[i_a];
            particles_b[i_a] = particles_b[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_b[i_b] = tmp;
        }
    }
}

void swap_vertical(uint i_a, uint i_b)
{
    Particle tmp;
    if (parity == 0)
    {
        if (particles_a[i_a].pos_z > particles_a[i_b].pos_z
            || (particles_a[i_a].pos_z == particles_a[i_b].pos_z
                && particles_a[i_a].pos_x > particles_a[i_b].pos_x))
        {
            tmp = particles_a[i_a];
            particles_a[i_a] = particles_a[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_a[i_b] = tmp;
        }
    } else {
        if (particles_b[i_a].pos_z > particles_b[i_b].pos_z
            || (particles_b[i_a].pos_z == particles_b[i_b].pos_z
                && particles_b[i_a].pos_x > particles_b[i_b].pos_x))
        {
            tmp = particles_b[i_a];
            particles_b[i_a] = particles_b[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_b[i_b] = tmp;
        }
    }
}

void main()
{
    uint i_a = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_WorkGroupSize.x * gl_NumWorkGroups.x;
    uint i_b = i_a + 1;
    if (sortStep == 0 && (gl_GlobalInvocationID.x % 2 == 0
    && gl_GlobalInvocationID.x < gl_WorkGroupSize.x * gl_NumWorkGroups.x - 1))
    {
        swap_horizontal(i_a, i_b);
        //if (parity == 0)
        //particles_a[i_a].pos_y = i_b;
        //else
        //particles_b[i_a].pos_y = i_b;
    }
    if (sortStep == 1 && (gl_GlobalInvocationID.x % 2 != 0
    && gl_GlobalInvocationID.x < gl_WorkGroupSize.x * gl_NumWorkGroups.x - 1))
    {
        swap_horizontal(i_a, i_b);
        //if (parity == 0)
        //particles_a[i_a].pos_y = i_b;
        //else
        //particles_b[i_a].pos_y = i_b;
    }
    i_b = i_a + gl_WorkGroupSize.x * gl_NumWorkGroups.x;
    if (sortStep == 2 && (gl_GlobalInvocationID.y % 2 == 0
    && gl_GlobalInvocationID.y < gl_WorkGroupSize.y * gl_NumWorkGroups.y - 1))
    {
        swap_vertical(i_a, i_b);
        //if (parity == 0)
        //particles_a[i_a].pos_y = i_b;
        //else
        //particles_b[i_a].pos_y = i_b;
    }
    if (sortStep == 3 && (gl_GlobalInvocationID.y % 2 != 0
    && gl_GlobalInvocationID.y < gl_WorkGroupSize.y * gl_NumWorkGroups.y - 1))
    {
        swap_vertical(i_a, i_b);
        //if (parity == 0)
        //particles_a[i_a].pos_y = i_b;
        //else
        //particles_b[i_a].pos_y = i_b;
    }
}
