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

    if (p.pos_x > 10)
    {
        p.pos_x = 0;
    }
    if (p.pos_y > 10)
    {
        p.pos_y = 0;
    }
    if (p.pos_z > 10)
    {
        p.pos_z = 0;
    }

    //set velocity to average velocity of neighbours
    vec3 vel = {0,0,0};
    vec3 center = {0,0,0};
    vec3 repulsion = {0, 0, 0};
    for (int x = -1 + int(i == 0); x < 2 && i < gl_WorkGroupSize.x * gl_NumWorkGroups.x - 1; x++)
    {
        for (int y = -1 + int(j == 0); y < 2 && j < gl_WorkGroupSize.y * gl_NumWorkGroups.y - 1; y++)
        {
            Particle neighbour = get_particle(i, j);
            vel += vec3(neighbour.vel_x, neighbour.vel_y, neighbour.vel_z);
            center += vec3(neighbour.pos_x, neighbour.pos_y, neighbour.pos_z);
            vec3 n_offset = vec3(p.pos_x, p.pos_y, p.pos_z)
                - vec3(neighbour.pos_x, neighbour.pos_y, neighbour.pos_z);
            repulsion += 1 / (length(n_offset) + 0.001) * n_offset;
        }
    }
    vel /= 9;
    center /= 9;

    //point velocity towards center of group
    //vel = 0.5 * vel + 0.5 * (center - vec3(p.pos_x, p.pos_y, p.pos_z));
    vel = 0.5 * vel + 0.5 * repulsion;

    p.vel_x = vel.x;
    p.vel_y = vel.y;
    p.vel_z = vel.z;
    write_particle(i, j, p);
}

void main()
{
    update_particle(int(gl_GlobalInvocationID.x), int(gl_GlobalInvocationID.y));
}
