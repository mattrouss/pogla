#version 450

const uint PARTICLE_NB = 1;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB, local_size_z = PARTICLE_NB) in;

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
    uint flock;
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

mat4 translate(vec3 t)
{
    return transpose(mat4(
            1, 0, 0, t.x,
            0, 1, 0, t.y,
            0, 0, 1, t.z,
            0, 0, 0, 1
        ));
}

Particle get_particle(int i, int j, int k)
{
    if (parity == 0)
    {
        return particles_a[i
            + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
            + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y];
    }
    else
    {
        return particles_b[i
        + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
        + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y];
    }
}

void write_particle(int i, int j, int k, Particle p)
{
    if (parity == 0)
    {
        particles_b[i
        + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
        + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y].transform = p.transform;
        particles_b[i
        + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
        + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y].vel = p.vel;
        particles_b[i
        + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
        + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y].flock = p.flock;
    }
    else
    {
        particles_a[i
        + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
        + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y].transform = p.transform;
        particles_a[i
        + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
        + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y].vel = p.vel;
        particles_a[i
        + j * gl_WorkGroupSize.x * gl_NumWorkGroups.x
        + k * gl_WorkGroupSize.x * gl_NumWorkGroups.x * gl_WorkGroupSize.y * gl_NumWorkGroups.y].flock = p.flock;
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

float rand(vec3 st, vec3 seed)
{
    return fract(sin(dot(st, seed))*43758.5453123);
}

vec3 randVec(vec3 st)
{
    return (vec3(rand(st, vec3(12.9898, 78.233, 52.568)),
        rand(st, vec3(15.5682, 45.2357, 24.526)),
        rand(st, vec3(56.235, 95.256, 12.7310))) - vec3(0.5,0.5,0.5))*2.0;
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
    const float cur_speed = length(p.vel);

    // Rescale speed if it exceeds max speed
    if (cur_speed > max_speed)
    {
        p.vel = max_speed * p.vel / cur_speed;
    }

    return p;
}

vec3 center_repulsion(vec3 position)
{
    return normalize(position) / (length(position) * length(position));
}

vec3 center_attraction(vec3 position, float radius)
{
    return int(length(position) > radius)
        * normalize(-position)
        * (length(position) - radius) * (length(position) - radius);
}

void update_particle(int i, int j, int k)
{
    Particle p = get_particle(i, j, k);
    vec3 pos = get_position(p);
    vec3 p_vel = p.vel;

    //set velocity to average velocity of neighbours
    vec3 vel = {0, 0, 0};
    vec3 center = {0, 0, 0};
    vec3 separation = {0, 0, 0};
    vec3 vel_normalized = normalize(p.vel);
    if (length(p.vel) == 0)
    {
        vel_normalized = p.vel;
    }

    int n_neighbours = 0;
    for (int x = -2 + int(i == 0) + int(i <= 1); x < 3 && x + i < gl_WorkGroupSize.x * gl_NumWorkGroups.x; x++)
    {
        for (int y = -2 + int(j == 0) + int(j <= 1); y < 3 && y + j < gl_WorkGroupSize.y * gl_NumWorkGroups.y; y++)
        {
            for (int z = -2 + int(k == 0) + int(k <= 1); z < 3 && z + k < gl_WorkGroupSize.z * gl_NumWorkGroups.z; z++)
            {
                if (x == 0 && y == 0 && z == 0)
                continue;

                Particle neighbour = get_particle(x + i, y + j, z + k);
                vec3 neighbour_pos = get_position(neighbour);
                vec3 n_offset = pos - neighbour_pos;
                float sqr_dist = dot(n_offset, n_offset);
                if (length(n_offset) > 0 && dot(vel_normalized, -normalize(n_offset)) >= 0.1)
                {
                    if (p.flock == neighbour.flock)
                    {
                        vel += neighbour.vel;
                        center += neighbour_pos;
                        n_neighbours += 1;
                    }


                    //if (sqr_dist < avoid_distance * avoid_distance)
                    separation += normalize(n_offset) * (1 / max(sqr_dist, 0.5));
                }
                else if (length(n_offset) > 0 && sqr_dist < avoid_distance * avoid_distance)
                separation += normalize(n_offset) * (1 / max(sqr_dist, 0.5));
            }
        }
    }
    if (n_neighbours > 0)
    {
        vel /= n_neighbours;
        center /= n_neighbours;
    }

    // Cohesion: point velocity towards center of group
    const vec3 v_cohesion = 0.1 * (center - pos);

    // Separation: avoid flockmates which are too close
    const vec3 v_separation = 10 * separation;

    // Alignment: align velocity to average flockmates
    const vec3 v_alignment = 1 * vel;

    vec3 acceleration = {0.0, 0.0, 0.0};
    vec3 randDir = randVec(pos) * 0.1;
    acceleration += (0.4*v_alignment + 0.6*v_cohesion) * int(n_neighbours > 0) + 0.7*v_separation;
    acceleration += randDir * 50.0;
    acceleration += center_repulsion(pos) * 10.0f;
    acceleration += center_attraction(pos, 20) * 0.0001;

    p.vel += deltatime * acceleration;

    //p = avoid_bounds(p);
    p = limit_speed(p);

    pos += deltatime * p.vel;

    mat4 translation = translate(deltatime * p.vel);

    p.transform = p.transform * translation;
    write_particle(i, j, k, p);
}

void main()
{
    update_particle(int(gl_GlobalInvocationID.x), int(gl_GlobalInvocationID.y),
        int(gl_GlobalInvocationID.z));
}
