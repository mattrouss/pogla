#include "particle_system.h"

ParticleSystem::ParticleSystem(size_t N)
    : N_(N)
{}

void ParticleSystem::init_system(std::shared_ptr<mygl::mesh> mesh)
{
    particle_mesh_ = mesh;
    init_particles();

    auto vertex_attribs = std::vector<GLuint>{0, 1, 2, 3, 4};

    glGenVertexArrays(1, &vao_);gl_err()
    glBindVertexArray(vao_);gl_err()

    // Create mesh data vbo
    GLuint vertex_buffer_id;
    glGenBuffers(1, &vertex_buffer_id);gl_err()

    auto verts = mesh->verts;
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);gl_err();
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(mygl::Vertex), verts.data(), GL_STATIC_DRAW);gl_err();

    // Create instance data vbo
    GLuint instance_vertex_buffer_id;
    glGenBuffers(1, &instance_vertex_buffer_id);

    glBindBuffer(GL_ARRAY_BUFFER, instance_vertex_buffer_id);gl_err();
    glBufferData(GL_ARRAY_BUFFER, positions_.size() * sizeof(mygl::Vec3), positions_.data(), GL_STATIC_DRAW);gl_err();


    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    //positions
    glVertexAttribPointer(vertex_attribs[0], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, pos)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[0]);gl_err();

    //normals
    glVertexAttribPointer(vertex_attribs[1], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, normal)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[1]);gl_err();

    //uv
    glVertexAttribPointer(vertex_attribs[2], 2, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, uv)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[2]);gl_err();

    //tangent
    glVertexAttribPointer(vertex_attribs[3], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, tangent)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[3]);gl_err();

    glBindBuffer(GL_ARRAY_BUFFER, instance_vertex_buffer_id);
    glVertexAttribPointer(vertex_attribs[4], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vec3), (void *)0);
    glEnableVertexAttribArray(vertex_attribs[4]);gl_err();
    glVertexAttribDivisor(vertex_attribs[4], 1); // Instanced attribute

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);gl_err()

}

void ParticleSystem::init_particles()
{
    positions_.resize(N_);

    auto grid_center = mygl::Vec3{{0.0, 0.0, 0.0}};
    float offset = 5.0f;

    size_t grid_width = static_cast<size_t>(std::sqrt(N_));
    for (auto i = 0u; i < grid_width; ++i)
    {
        for (auto j = 0u; j < grid_width; ++j)
        {
            positions_[j + grid_width * i] = grid_center + mygl::Vec3{{i * offset, 0.0, j * offset}};
        }
    }
}

void ParticleSystem::render() const
{
    auto transform_id = glGetUniformLocation(prog_->prog_id(), "model_matrix");gl_err()
    glUniformMatrix4fv(transform_id, 1, GL_FALSE, particle_mesh_->get_transform().transpose().data.data());gl_err()
    glBindVertexArray(vao_);gl_err()
    glDrawArraysInstanced(GL_TRIANGLES, 0, particle_mesh_->verts.size() * 3, N_);gl_err()
    glBindVertexArray(0);gl_err()

}
