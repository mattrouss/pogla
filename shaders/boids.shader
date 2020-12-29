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

Particle limit_bounds(Particle p)
{
    const float upper_bound = 20;
    const float lower_bound = -20;

    if (p.pos_x > upper_bound)
    {
        p.pos_x = lower_bound;
    }
    if (p.pos_y > upper_bound)
    {
        p.pos_y = lower_bound;
    }
    if (p.pos_z > upper_bound)
    {
        p.pos_z = lower_bound;
    }
    if (p.pos_x < lower_bound)
    {
        p.pos_x = upper_bound;
    }
    if (p.pos_y < lower_bound)
    {
        p.pos_y = upper_bound;
    }
    if (p.pos_z < lower_bound)
    {
        p.pos_z = upper_bound;
    }

    return p;
    
}

Particle avoid_bounds(Particle p)
{
    const float upper_bound = 20;
    const float lower_bound = -20;

    const float margin = 1.5;
    const float turnaround_factor = 1.0f;
    if (p.pos_x > upper_bound - margin)
    {
        p.vel_x -= turnaround_factor;
    }
    if (p.pos_y > upper_bound - margin)
    {
        p.vel_y -= turnaround_factor;
    }
    if (p.pos_z > upper_bound - margin)
    {
        p.vel_z -= turnaround_factor;
    }
    if (p.pos_x < lower_bound + margin)
    {
        p.vel_x += turnaround_factor;
    }
    if (p.pos_y < lower_bound + margin)
    {
        p.vel_y += turnaround_factor;
    }
    if (p.pos_z < lower_bound + margin)
    {
        p.vel_z += turnaround_factor;
    }

    return p;
}

Particle limit_speed(Particle p)
{
    const float max_speed = 5.0;

    vec3 p_vel = vec3(p.vel_x, p.vel_y, p.vel_z);
    const float cur_speed = length(p_vel);

    // Rescale speed if it exceeds max speed
    if (cur_speed > max_speed)
    {
        p.vel_x = max_speed * p.vel_x / cur_speed;
        p.vel_y = max_speed * p.vel_y / cur_speed;
        p.vel_z = max_speed * p.vel_z / cur_speed;
    }

    return p;
}

void update_particle(int i, int j)
{
    Particle p = get_particle(i, j);
    vec3 p_vel = vec3(p.vel_x, p.vel_y, p.vel_z);

    //set velocity to average velocity of neighbours
    vec3 vel = {0, 0, 0};
    vec3 center = {0, 0, 0};
    vec3 repulsion = {0, 0, 0};
    for (int x = -1 + int(i == 0); x < 2 && x + i < gl_WorkGroupSize.x * gl_NumWorkGroups.x - 1; x++)
    {
        for (int y = -1 + int(j == 0); y < 2 && y + j < gl_WorkGroupSize.y * gl_NumWorkGroups.y - 1; y++)
        {
            if (x == 0 && y == 0)
                continue;
            Particle neighbour = get_particle(x + i, y + j);
            vel += vec3(neighbour.vel_x, neighbour.vel_y, neighbour.vel_z);
            center += vec3(neighbour.pos_x, neighbour.pos_y, neighbour.pos_z);
            vec3 n_offset = vec3(p.pos_x, p.pos_y, p.pos_z)
                - vec3(neighbour.pos_x, neighbour.pos_y, neighbour.pos_z);
            repulsion += 1 / (length(n_offset) + 0.001) * n_offset;
        }
    }
    vel /= 9;
    center /= 9;

    // Cohesion: point velocity towards center of group
    const vec3 v_cohesion = 0.01 * (center - vec3(p.pos_x, p.pos_y, p.pos_z));

    // Repulsion: avoid flockmates which are too close
    const vec3 v_repulsion = 0.6 * repulsion;

    // Alignment: align velocity to average flockmates
    const vec3 v_alignment = 0.0125 * (vel - p_vel);

    p_vel += v_cohesion + v_repulsion + v_alignment;

    p.vel_x = p_vel.x;
    p.vel_y = p_vel.y;
    p.vel_z = p_vel.z;

    p = limit_bounds(p);
    p = limit_speed(p);

    p.pos_x += deltatime * p.vel_x;
    p.pos_y += deltatime * p.vel_y;
    p.pos_z += deltatime * p.vel_z;

    write_particle(i, j, p);
}

void main()
{
    update_particle(int(gl_GlobalInvocationID.x), int(gl_GlobalInvocationID.y));
}
