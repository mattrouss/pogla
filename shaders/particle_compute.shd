#version 450

const uint PARTICLE_NB = 50;

layout (local_size_x = PARTICLE_NB) in;

uniform float time;
const float period = 2.0;

struct Particle
{
    float pos_x, pos_y, pos_z;
};

layout (std430, binding=1) buffer particle_pos_buffer
{
    Particle particles[];
};


void update_particle(int i)
{
    float x = particles[i].pos_x;
    float z = particles[i].pos_z;
    particles[i].pos_y = sin(x + z + time / 1000);
}

void main()
{
    update_particle(int(gl_GlobalInvocationID.x));
}
