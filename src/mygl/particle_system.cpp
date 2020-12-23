#include "particle_system.h"

namespace mygl
{
    static void compute_workgroup_xy(const size_t N, size_t& x, size_t& y)
    {
        int result = N/2 - 1;
        int best = N;
        for (; result >= 2; result--)
        {
            if (N%result == 0 && result + N/result < best + N/best)
            {
                best = result;
            }
        }
        int x_pad = best + (4 - best%4);
        int y_pad = N/best + (4 - (N/best)%4);
        x = x_pad;
        y = y_pad;
    }

    ParticleSystem::ParticleSystem(size_t N)
        : N_(N)
    {
        compute_workgroup_xy(N, N_x_, N_y_);
        N_ = N_x_ * N_y_;
    }

    void ParticleSystem::init_system(mygl::Program* display_prog, mygl::Program* compute_prog, std::shared_ptr<mygl::mesh> mesh)
    {
        init_system(display_prog, compute_prog, nullptr, mesh);
    }

    void ParticleSystem::init_system(mygl::Program* display_prog,
                     mygl::Program* compute_prog,
                     mygl::Program* sort_prog,
                     std::shared_ptr<mygl::mesh> mesh)
    {
        display_prog_ = display_prog;
        compute_prog_ = compute_prog;
        sort_prog_ = sort_prog;
        particle_mesh_ = mesh;

        init_particles();

        auto vertex_attribs = std::vector<GLuint>{0, 1, 2, 3, 4, 5};

        glGenVertexArrays(1, &vao_);gl_err()
        glBindVertexArray(vao_);gl_err()

        // Create mesh data vbo
        GLuint vertex_buffer_id;
        glGenBuffers(1, &vertex_buffer_id);gl_err()

        auto verts = mesh->verts;
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);gl_err();
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(mygl::Vertex), verts.data(), GL_STATIC_DRAW);gl_err();


        // Create instance data ssbo
        GLuint instance_vertex_buffer_id_a;
        GLuint instance_vertex_buffer_id_b;
        glGenBuffers(1, &instance_vertex_buffer_id_a);
        glGenBuffers(1, &instance_vertex_buffer_id_b);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_vertex_buffer_id_a);gl_err();
        glBufferData(GL_SHADER_STORAGE_BUFFER, positions_a_.size() * sizeof(mygl::Vec3), positions_a_.data(), GL_STATIC_DRAW);gl_err();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, instance_vertex_buffer_id_a);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_vertex_buffer_id_b);gl_err();
        glBufferData(GL_SHADER_STORAGE_BUFFER, positions_b_.size() * sizeof(mygl::Vec3), positions_b_.data(), GL_STATIC_DRAW);gl_err();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, instance_vertex_buffer_id_b);


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

        glBindBuffer(GL_ARRAY_BUFFER, instance_vertex_buffer_id_a);
        glVertexAttribPointer(vertex_attribs[4], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vec3), (void *)0);
        glEnableVertexAttribArray(vertex_attribs[4]);gl_err();
        glVertexAttribDivisor(vertex_attribs[4], 1); // Instanced attribute

        glBindBuffer(GL_ARRAY_BUFFER, instance_vertex_buffer_id_b);
        glVertexAttribPointer(vertex_attribs[5], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vec3), (void *)0);
        glEnableVertexAttribArray(vertex_attribs[5]);gl_err();
        glVertexAttribDivisor(vertex_attribs[5], 1); // Instanced attribute

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);gl_err()
    }

    void ParticleSystem::init_particles()
    {
        positions_a_.resize(N_);
        positions_b_.resize(N_);

        auto grid_center = mygl::Vec3{{0.0, 0.0, 0.0}};
        float offset = 5.0f;

        size_t grid_width = static_cast<size_t>(std::sqrt(N_));
        size_t grid_length = offset * (grid_width - 1);
        for (auto i = 0u; i < grid_width; ++i)
        {
            for (auto j = 0u; j < grid_width; ++j)
            {
                positions_a_[j + grid_width * i] = grid_center
                   - mygl::Vec3{{grid_length / 2.0f, 0, grid_length / 2.0f}}
                   + mygl::Vec3{{i * offset, 0.0, j * offset}};
                positions_b_[j + grid_width * i] = grid_center
                   - mygl::Vec3{{grid_length / 2.0f, 0, grid_length / 2.0f}}
                   + mygl::Vec3{{i * offset, 0.0, j * offset}};
            }
        }
    }

    void ParticleSystem::render()
    {
        if (sort_prog_ != nullptr)
        {
            sort_prog_->use();

            glBindVertexArray(vao_);
            gl_err()
            glDispatchCompute(N_x_, N_y_, 1);
            gl_err();
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glBindVertexArray(0);
            gl_err();
        }
        // Run compute program
        compute_prog_->use();

        auto time = glutGet(GLUT_ELAPSED_TIME);
        GLint time_id;
        time_id = glGetUniformLocation(compute_prog_->prog_id(), "time");gl_err();
        glUniform1f(time_id, time);gl_err();
        GLint parity_id;
        parity_id = glGetUniformLocation(compute_prog_->prog_id(), "parity");gl_err();
        glUniform1ui(parity_id, iteration_parity);gl_err();
        iteration_parity = (iteration_parity + 1) % 2;

        glBindVertexArray(vao_);gl_err();
        glDispatchCompute(N_x_, N_y_, 1);gl_err();
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glBindVertexArray(0);gl_err();

        // Run display program
        display_prog_->use();
        auto transform_id = glGetUniformLocation(display_prog_->prog_id(), "model_matrix");gl_err()
        glUniformMatrix4fv(transform_id, 1, GL_FALSE, particle_mesh_->get_transform().transpose().data.data());gl_err()
        glBindVertexArray(vao_);gl_err()
        glDrawArraysInstanced(GL_TRIANGLES, 0, particle_mesh_->verts.size() * 3, N_);gl_err()
        glBindVertexArray(0);gl_err()

    }
}
