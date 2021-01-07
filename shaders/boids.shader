#version 450

const uint PARTICLE_NB = 1;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
uniform float deltatime;
uniform uint parity;
const float period = 2.0;

// Boid params
const float avoid_distance = 10.0f;

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
    const float upper_bound = 100;
    const float lower_bound = -100;

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
    vec3 separation = {0, 0, 0};
    vec3 vel_normalized = normalize(p_vel);

    for (int x = -2 + int(i == 0) + int(i < 1); x < 3 && x + i < gl_WorkGroupSize.x * gl_NumWorkGroups.x - 2; x++)
    {
        for (int y = -2 + int(j == 0) + int(i < 1); y < 3 && y + j < gl_WorkGroupSize.y * gl_NumWorkGroups.y - 2; y++)
        {
            if (x == 0 && y == 0)
                continue;

            Particle neighbour = get_particle(x + i, y + j);
            vec3 n_offset = vec3(p.pos_x, 0, p.pos_z)
            - vec3(neighbour.pos_x, 0, neighbour.pos_z);
            float sqr_dist = dot(n_offset, n_offset);
            //if (dot(vel_normalized, -normalize(n_offset)) >= 0
            //    || sqr_dist < 1)
            {

                vel += vec3(neighbour.vel_x, 0, neighbour.vel_z);
                center += vec3(neighbour.pos_x, 0, neighbour.pos_z);



                //if (sqr_dist < avoid_distance * avoid_distance)
                    separation += normalize(n_offset) * (1 / sqr_dist + 0.000001);
            }
        }
    }
    vel /= 9;
    center /= 9;

    // Cohesion: point velocity towards center of group
    const vec3 v_cohesion = 0.1 * (center - vec3(p.pos_x, 0, p.pos_z));

    // Separation: avoid flockmates which are too close
    const vec3 v_separation = 100 * separation;

    // Alignment: align velocity to average flockmates
    const vec3 v_alignment = 1 * vel;

    vec3 acceleration = {0.0, 0.0, 0.0};
    acceleration += 0*v_alignment + 0*v_cohesion + 1*v_separation;

    p_vel += acceleration;

    p.vel_x = p_vel.x;
    p.vel_y = p_vel.y;
    p.vel_z = p_vel.z;

    p = avoid_bounds(p);
    p = limit_speed(p);

    p.pos_x += deltatime * p.vel_x;
    p.pos_y += 0;
    p.pos_z += deltatime * p.vel_z;

    write_particle(i, j, p);
}

void main()
{
    update_particle(int(gl_GlobalInvocationID.x), int(gl_GlobalInvocationID.y));
}
