#version 450

const uint PARTICLE_NB = 1;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
uniform uint parity;
uniform uint sortStep;
const float period = 2.0;

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

bool check_horizontal(vec3 pos_a, vec3 pos_b)
{
    return pos_a.x > pos_b.x || (pos_a.x == pos_b.x && pos_a.z > pos_b.z)
        || (pos_a.x == pos_b.x && pos_a.z <= pos_b.z && pos_a.y > pos_b.y);
}

void swap_horizontal(uint i_a, uint i_b)
{
    Particle tmp;
    if (parity == 0)
    {
        vec3 pos_a = get_position(particles_a[i_a]);
        vec3 pos_b = get_position(particles_a[i_b]);
        if (check_horizontal(pos_a, pos_b))
        {
            tmp = particles_a[i_a];
            particles_a[i_a] = particles_a[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_a[i_b] = tmp;
        }
    } else {
        vec3 pos_a = get_position(particles_b[i_a]);
        vec3 pos_b = get_position(particles_b[i_b]);
        if (check_horizontal(pos_a, pos_b))
        {
            tmp = particles_b[i_a];
            particles_b[i_a] = particles_b[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_b[i_b] = tmp;
        }
    }
}

bool check_vertical(vec3 pos_a, vec3 pos_b)
{
    return pos_a.z > pos_b.z || (pos_a.z == pos_b.z && pos_a.y > pos_b.y)
        || (pos_a.z == pos_b.z && pos_a.y <= pos_b.y && pos_a.x > pos_b.x);
}

void swap_vertical(uint i_a, uint i_b)
{
    Particle tmp;
    if (parity == 0)
    {
        vec3 pos_a = get_position(particles_a[i_a]);
        vec3 pos_b = get_position(particles_a[i_b]);
        if (check_vertical(pos_a, pos_b))
        {
            tmp = particles_a[i_a];
            particles_a[i_a] = particles_a[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_a[i_b] = tmp;
        }
    } else {
        vec3 pos_a = get_position(particles_b[i_a]);
        vec3 pos_b = get_position(particles_b[i_b]);
        if (check_vertical(pos_a, pos_b))
        {
            tmp = particles_b[i_a];
            particles_b[i_a] = particles_b[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_b[i_b] = tmp;
        }
    }
}

bool check_depth(vec3 pos_a, vec3 pos_b)
{
    return pos_a.y > pos_b.y || (pos_a.y == pos_b.y && pos_a.x > pos_b.x)
        || (pos_a.y == pos_b.y && pos_a.x <= pos_b.x && pos_a.z > pos_b.z);
}

void swap_depth(uint i_a, uint i_b)
{
    Particle tmp;
    if (parity == 0)
    {
        vec3 pos_a = get_position(particles_a[i_a]);
        vec3 pos_b = get_position(particles_a[i_b]);
        if (check_depth(pos_a, pos_b))
        {
            tmp = particles_a[i_a];
            particles_a[i_a] = particles_a[i_b];
            //particles_a[i_a].pos_y = i_b;
            particles_a[i_b] = tmp;
        }
    } else {
        vec3 pos_a = get_position(particles_b[i_a]);
        vec3 pos_b = get_position(particles_b[i_b]);
        if (check_depth(pos_a, pos_b))
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
    i_b = i_a + gl_WorkGroupSize.x * gl_NumWorkGroups.x
        * gl_WorkGroupSize.y * gl_NumWorkGroups.y;
    if (sortStep == 4 && (gl_GlobalInvocationID.z % 2 == 0
    && gl_GlobalInvocationID.z < gl_WorkGroupSize.z * gl_NumWorkGroups.z - 1))
    {
        swap_depth(i_a, i_b);
        //if (parity == 0)
        //particles_a[i_a].pos_y = i_b;
        //else
        //particles_b[i_a].pos_y = i_b;
    }
    if (sortStep == 5 && (gl_GlobalInvocationID.z % 2 != 0
    && gl_GlobalInvocationID.z < gl_WorkGroupSize.z * gl_NumWorkGroups.z - 1))
    {
        swap_depth(i_a, i_b);
        //if (parity == 0)
        //particles_a[i_a].pos_y = i_b;
        //else
        //particles_b[i_a].pos_y = i_b;
    }
}
