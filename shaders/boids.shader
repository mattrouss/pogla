#version 450

const uint PARTICLE_NB = 1;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
uniform float deltatime;
uniform uint parity;
const float period = 2.0;

// Boid params
const float avoid_distance = 1.5f;

struct Particle
{
    mat4 transform;
    vec3 vel;
    float padding;
};

layout (std430, binding=1) buffer particle_pos_buffer_a
{
    Particle particles_a[];
};

layout (std430, binding=2) buffer particle_pos_buffer_b
{
    Particle particles_b[];
};

vec3 get_position(Particle p)
{
    return vec3(p.transform[3][0], p.transform[3][1], p.transform[3][2]);
}

Particle set_position(Particle p, vec3 pos)
{
    p.transform[3][0] = pos.x;
    p.transform[3][1] = pos.y;
    p.transform[3][2] = pos.z;

    return p;
}

mat4 translate(mat4 m, vec3 t)
{
    m[3].xyz += t;
    return m;
}


mat4 rot_y(float angle)
{
    float cos_a = cos(angle);
    float sin_a = sin(angle);
    return mat4(
        cos_a, 0, sin_a, 0,
             0, 1,     0, 0,
        -sin_a, 0, cos_a, 0,
             0, 0,     0, 1
            );
}

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
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].transform = p.transform;
        particles_b[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel = p.vel;
    }
    else
    {
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].transform = p.transform;
        particles_a[i + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x].vel = p.vel;
    }
}

Particle limit_bounds(Particle p)
{
    const float upper_bound = 100;
    const float lower_bound = -100;

    vec3 pos = get_position(p);
    if (pos.x > upper_bound)
    {
        pos.x = lower_bound;
    }
    if (pos.y > upper_bound)
    {
        pos.y = lower_bound;
    }
    if (pos.z > upper_bound)
    {
        pos.z = lower_bound;
    }
    if (pos.x < lower_bound)
    {
        pos.x = upper_bound;
    }
    if (pos.y < lower_bound)
    {
        pos.y = upper_bound;
    }
    if (pos.z < lower_bound)
    {
        pos.z = upper_bound;
    }

    return set_position(p, pos);
    
}

float rand(vec2 st, vec2 seed)
{
    return fract(sin(dot(st.xy, seed))*43758.5453123);
}

vec2 randVec(vec2 st)
{
    return (vec2(rand(st, vec2(12.9898, 78.233)),
        rand(st, vec2(15.5682, 45.2357))) - vec2(0.5,0.5))*2.0;
}

Particle avoid_bounds(Particle p)
{
    const float upper_bound = 20;
    const float lower_bound = -20;

    const float margin = 1.5f;
    const float turnaround_factor = 0.05f;
    vec3 pos = get_position(p);
    if (pos.x > upper_bound - margin)
    {
        p.vel.x -= turnaround_factor;
    }
    if (pos.y> upper_bound - margin)
    {
        p.vel.y -= turnaround_factor;
    }
    if (pos.z> upper_bound - margin)
    {
        p.vel.z -= turnaround_factor;
    }
    if (pos.x< lower_bound + margin)
    {
        p.vel.x += turnaround_factor;
    }
    if (pos.y< lower_bound + margin)
    {
        p.vel.y += turnaround_factor;
    }
    if (pos.z< lower_bound + margin)
    {
        p.vel.z += turnaround_factor;
    }

    return p;
}

Particle limit_speed(Particle p)
{
    const float max_speed = 5.0;

    vec3 p_vel = p.vel;
    const float cur_speed = length(p_vel);

    // Rescale speed if it exceeds max speed
    if (cur_speed > max_speed)
    {
        p.vel.x = max_speed * p.vel.x / cur_speed;
        p.vel.y = max_speed * p.vel.y / cur_speed;
        p.vel.z = max_speed * p.vel.z / cur_speed;
    }

    return p;
}

vec2 center_repulsion(vec2 position)
{
    return normalize(position) / (length(position) * length(position));
}

vec2 center_attraction(vec2 position, float radius)
{
    return int(length(position) > radius)
        * normalize(-position)
        * (length(position) - radius) * (length(position) - radius);
}

void update_particle(int i, int j)
{
    Particle p = get_particle(i, j);
    vec3 pos = get_position(p);
    vec3 p_vel = p.vel;

    //set velocity to average velocity of neighbours
    vec3 vel = {0, 0, 0};
    vec3 center = {0, 0, 0};
    vec3 separation = {0, 0, 0};
    vec3 vel_normalized = normalize(p_vel);
    if (length(p_vel) == 0)
    {
        vel_normalized = p_vel;
    }

    int n_neighbours = 0;
    for (int x = -2 + int(i == 0) + int(i <= 1); x < 3 && x + i < gl_WorkGroupSize.x * gl_NumWorkGroups.x; x++)
    {
        for (int y = -2 + int(j == 0) + int(i <= 1); y < 3 && y + j < gl_WorkGroupSize.y * gl_NumWorkGroups.y; y++)
        {
            if (x == 0 && y == 0)
                continue;

            Particle neighbour = get_particle(x + i, y + j);
            vec3 neighbour_pos = get_position(neighbour);
            vec3 n_offset = vec3(pos.x, 0, pos.z)
                - vec3(neighbour_pos.x, 0, neighbour_pos.z);
            float sqr_dist = dot(n_offset, n_offset);
            if (length(n_offset) > 0 && dot(vel_normalized, -normalize(n_offset)) >= 0.1)
            {

                vel += vec3(neighbour.vel.x, 0, neighbour.vel.z);
                center += vec3(neighbour_pos[0], 0, neighbour_pos[2]);



                //if (sqr_dist < avoid_distance * avoid_distance)
                    separation += normalize(n_offset) * (1 / max(sqr_dist, 0.5));
                n_neighbours += 1;
            }
            else if (length(n_offset) > 0 && sqr_dist < avoid_distance * avoid_distance)
                separation += normalize(n_offset) * (1 / max(sqr_dist, 0.5));
        }
    }
    if (n_neighbours > 0)
    {
        vel /= n_neighbours;
        center /= n_neighbours;
    }

    // Cohesion: point velocity towards center of group
    const vec3 v_cohesion = 0.1 * (center - vec3(pos.x, 0, pos.z));

    // Separation: avoid flockmates which are too close
    const vec3 v_separation = 10 * separation;

    // Alignment: align velocity to average flockmates
    const vec3 v_alignment = 1 * vel;

    vec3 acceleration = {0.0, 0.0, 0.0};
    vec2 randDir = randVec(vec2(pos.x, pos.y)) * 0.1;
    acceleration += (1.5*v_alignment + 1*v_cohesion) * int(n_neighbours > 0) + 1*v_separation;
    acceleration.xz += randDir * 50.0;
    acceleration.xz += center_repulsion(vec2(pos.x, pos.z)) * 10.0f;
    acceleration.xz += center_attraction(vec2(pos.x, pos.z), 20) * 0.001;

    p_vel += deltatime * acceleration;

    p.vel.x = p_vel.x;
    p.vel.y = p_vel.y;
    p.vel.z = p_vel.z;

    //p = avoid_bounds(p);
    p = limit_speed(p);

    pos.x += deltatime * p.vel.x;
    pos.y += 0;
    pos.z += deltatime * p.vel.z;

    p.transform = p.transform * rot_y(0.01);

    p = set_position(p, pos);

    write_particle(i, j, p);
}

void main()
{
    /*
    if (int(gl_GlobalInvocationID.x) == 0 && int(gl_GlobalInvocationID.y) == 0)
    {
        Particle p = get_particle(int(gl_GlobalInvocationID.x), int(gl_GlobalInvocationID.y));
        vec3 pos = get_position(p);
        p.transform = translate(p.transform, vec3(0, 20, 0));


    }
    */
    update_particle(int(gl_GlobalInvocationID.x), int(gl_GlobalInvocationID.y));
}
