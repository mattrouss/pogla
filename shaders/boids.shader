#version 450

const uint PARTICLE_NB = 5;

layout (local_size_x = PARTICLE_NB, local_size_y = PARTICLE_NB) in;

uniform float time;
uniform uint parity;
const float period = 2.0;

struct Particle
{
    float pos_x, pos_y, pos_z;
};

layout (std430, binding=1) buffer particle_pos_buffer_a
{
    Particle particles_a[];
};

layout (std430, binding=2) buffer particle_pos_buffer_b
{
    Particle particles_b[];
};

Particle get_particle(int i)
{
    if (parity == 0)
    {
        return particles_a[i];
    }
    else
    {
        return particles_b[i];
    }
}

void write_particle(int i, vec3 pos)
{
    if (parity == 0)
    {
        particles_b[i].pos_x = pos.x;
        particles_b[i].pos_y = pos.y;
        particles_b[i].pos_z = pos.z;
    }
    else
    {
        particles_a[i].pos_x = pos.x;
        particles_a[i].pos_y = pos.y;
        particles_a[i].pos_z = pos.z;
    }
}

void update_particle(int i)
{
    float x = get_particle(i).pos_x;
    float z = get_particle(i).pos_z;

    float x_sum = 0;

    x_sum += (x - get_particle(i - 1).pos_x) * (1/(0.001 + abs(x - get_particle(i - 1).pos_x)));
    x_sum += (get_particle(i + 1).pos_x - x) * (1/(0.001 + abs(-x + get_particle(i + 1).pos_x)));
    write_particle(i, vec3((x_sum)/2, sin(x + z + time / 1000), z));
    //write_particle(i, vec3(x, sin(x + z + time / 1000), z));
}

void main()
{
    update_particle(int(gl_GlobalInvocationID.x));
}
