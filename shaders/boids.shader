#version 450

const uint PARTICLE_NB = 1;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
uniform float deltatime;
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

Particle get_particle(int i, int j)
{
    if (parity == 0)
    {
        return particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x];
    }
    else
    {
        return particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x];
    }
}

void write_particle(int i, int j, Particle p)
{
    if (parity == 0)
    {
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].pos_x = p.pos_x;
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].pos_y = p.pos_y;
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].pos_z = p.pos_z;
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel_x = p.vel_x;
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel_y = p.vel_y;
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel_z = p.vel_z;
    }
    else
    {
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].pos_x = p.pos_x;
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].pos_y = p.pos_y;
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].pos_z = p.pos_z;
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel_x = p.vel_x;
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel_y = p.vel_y;
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel_z = p.vel_z;
    }
}

void update_particle(int i, int j)
{
    Particle p = get_particle(i, j);

    p.pos_x += deltatime * p.vel_x;
    p.pos_y += deltatime * p.vel_y;
    p.pos_z += deltatime * p.vel_z;

    write_particle(i, j, p);
}

void main()
{
    update_particle(int(gl_GlobalInvocationID.x), int(gl_GlobalInvocationID.y));
}
