#include <iomanip>
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
        ssbo_a_ = instance_vertex_buffer_id_a;
        ssbo_b_ = instance_vertex_buffer_id_b;

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_vertex_buffer_id_a);gl_err();
        glBufferData(GL_SHADER_STORAGE_BUFFER, particles_a_.size() * sizeof(mygl::Particle), particles_a_.data(), GL_STATIC_DRAW);gl_err();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, instance_vertex_buffer_id_a);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_vertex_buffer_id_b);gl_err();
        glBufferData(GL_SHADER_STORAGE_BUFFER, particles_b_.size() * sizeof(mygl::Particle), particles_b_.data(), GL_STATIC_DRAW);gl_err();
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
        glVertexAttribPointer(vertex_attribs[4], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Particle), (void *)0);
        glEnableVertexAttribArray(vertex_attribs[4]);gl_err();
        glVertexAttribDivisor(vertex_attribs[4], 1); // Instanced attribute

        glBindBuffer(GL_ARRAY_BUFFER, instance_vertex_buffer_id_b);
        glVertexAttribPointer(vertex_attribs[5], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Particle), (void *)0);
        glEnableVertexAttribArray(vertex_attribs[5]);gl_err();
        glVertexAttribDivisor(vertex_attribs[5], 1); // Instanced attribute

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);gl_err()
    }

    void ParticleSystem::print_ssbo()
    {
        if (iteration_parity == 0)
        {
            print_buffer("A", particles_a_);
            print_sorted("A", particles_a_);
        }
        else
        {
            print_buffer("B", particles_b_);
            print_sorted("B", particles_b_);
        }
    }

    void ParticleSystem::retrieve_ssbo()
    {

            glGetNamedBufferSubData(ssbo_a_, 0, particles_a_.size() * sizeof(Particle), particles_a_.data());
            gl_err();


            glGetNamedBufferSubData(ssbo_b_, 0, particles_b_.size() * sizeof(Particle), particles_b_.data());
            gl_err();


    }

    void ParticleSystem::print_buffer(std::string name, std::vector<mygl::Particle> buffer)
    {
        size_t grid_width = N_x_;
        std::cout << "\nBuffer: " << name << "\n";
        for (size_t y = 0; y < N_y_; y++)
        {
            std::cout << "[";
            for (size_t x = 0; x < N_x_; x++)
            {
                std::cout
                    << "[" << buffer[x + grid_width * y].pos_ << "] ";
                    //<< ":" << buffer[x + grid_width * y].velocity_ << "] ";
            }
            std::cout << "]\n";
        }
        std::cout << "\n\n";
        for (size_t y = 0; y < N_y_; y++)
        {
            std::cout << "[";
            for (size_t x = 0; x < N_x_; x++)
            {
                std::cout
                        << "[" << buffer[x + grid_width * y].velocity_ << "] ";
                        //<< ":" << buffer[x + grid_width * y].velocity_ << "] ";
            }
            std::cout << "]\n";
        }
    }

    void ParticleSystem::print_sorted(std::string name, std::vector<mygl::Particle> buffer)
    {
        std::cout << "\nBuffer: " << name << "\n";
        if (check_sorted(buffer))
        {
            std::cout << "SORTED.\n";
        }
        else
        {
            std::cout << "NOT SORTED.\n";
        }
    }

    bool ParticleSystem::check_sorted(std::vector<mygl::Particle> buffer)
    {
        size_t grid_width = N_x_;

        bool sorted = true;
        for (size_t y = 0; y < N_y_ - 1; y++)
        {
            for (size_t x = 0; x < N_x_ - 1; x++)
            {
                if (buffer[(x + 1) + grid_width * y].pos_[0]
                    < buffer[x + grid_width * y].pos_[0])
                {
                    sorted = false;
                }
                else if (buffer[x + grid_width * (y + 1)].pos_[2]
                    < buffer[x + grid_width * y].pos_[2])
                {
                    sorted = false;
                }
                else if (buffer[(x + 1) + grid_width * y].pos_[0]
                         == buffer[x + grid_width * y].pos_[0]
                         && buffer[(x + 1) + grid_width * y].pos_[2]
                            < buffer[x + grid_width * y].pos_[2])
                {
                    sorted = false;
                }
                else if (buffer[x + grid_width * (y + 1)].pos_[2]
                         == buffer[x + grid_width * y].pos_[2]
                         && buffer[x + grid_width * (y + 1)].pos_[0]
                            < buffer[x + grid_width * y].pos_[0])
                {
                    sorted = false;
                }
            }
        }

        return sorted;
    }

    void ParticleSystem::init_particles()
    {
        particles_a_.resize(N_);
        particles_b_.resize(N_);

        auto grid_center = mygl::Vec3{{0.0, 0.0, 0.0}};
        float offset = 5.0f;

        size_t grid_width = N_x_;
        size_t grid_length = offset * (grid_width - 1);
        for (auto y = 0u; y < N_y_; ++y)
        {
            for (auto x = 0u; x < N_x_;++x)
            {
                auto pos = grid_center
                   - mygl::Vec3{{grid_length / 2.0f, 0, grid_length / 2.0f}}
                   + mygl::Vec3{{x * offset, 0.0, y * offset}};
                auto vel = Vec3{{0.0f, 0.0f, 6.0f}};

                auto p = Particle(pos, vel);

                particles_a_[x + grid_width * y] = p;
                particles_b_[x + grid_width * y] = p;
            }
        }
    }

    void ParticleSystem::render(float deltatime)
    {
        GLint parity_id;
        if (sort_prog_ != nullptr)
        {
            sort_prog_->use();
            GLint step_id;

            parity_id = glGetUniformLocation(sort_prog_->prog_id(), "parity");gl_err();
            glUniform1ui(parity_id, iteration_parity);gl_err();

            step_id = glGetUniformLocation(sort_prog_->prog_id(), "sortStep");gl_err();

            //std::cout << "====================\n";
            do
            {
                //std::cout << "Execute sort.\n";
                glBindVertexArray(vao_);
                gl_err()
                glUniform1ui(step_id, 0);gl_err();
                glDispatchCompute(N_x_, N_y_, 1);
                gl_err();
                glUniform1ui(step_id, 1);gl_err();
                glMemoryBarrier(GL_ALL_BARRIER_BITS);
                glDispatchCompute(N_x_, N_y_, 1);
                gl_err();
                glUniform1ui(step_id, 2);gl_err();
                glMemoryBarrier(GL_ALL_BARRIER_BITS);
                glDispatchCompute(N_x_, N_y_, 1);
                gl_err();
                glUniform1ui(step_id, 3);gl_err();
                glMemoryBarrier(GL_ALL_BARRIER_BITS);
                glDispatchCompute(N_x_, N_y_, 1);
                gl_err();
                glMemoryBarrier(GL_ALL_BARRIER_BITS);
                retrieve_ssbo();
                //print_ssbo();
            } while (not check_sorted(iteration_parity == 0 ? particles_a_ : particles_b_));
            glBindVertexArray(0);
            gl_err();
            //print_ssbo();
        }
        // Run compute program
        compute_prog_->use();

        auto time = glutGet(GLUT_ELAPSED_TIME);
        GLint time_id;
        time_id = glGetUniformLocation(compute_prog_->prog_id(), "time");gl_err();
        glUniform1f(time_id, time);gl_err();
        GLint deltatime_id = glGetUniformLocation(compute_prog_->prog_id(), "deltatime");gl_err();
        glUniform1f(deltatime_id, deltatime);gl_err();


        parity_id = glGetUniformLocation(compute_prog_->prog_id(), "parity");gl_err();
        glUniform1ui(parity_id, iteration_parity);gl_err();
        iteration_parity = (iteration_parity + 1) % 2;

        glBindVertexArray(vao_);gl_err();
        glDispatchCompute(N_x_, N_y_, 1);gl_err();
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glBindVertexArray(0);gl_err();

        retrieve_ssbo();
        //print_ssbo();
        //std::cout << "===================\n";

        // Run display program
        display_prog_->use();
        auto transform_id = glGetUniformLocation(display_prog_->prog_id(), "model_matrix");gl_err()
        glUniformMatrix4fv(transform_id, 1, GL_FALSE, particle_mesh_->get_transform().transpose().data.data());gl_err()
        glBindVertexArray(vao_);gl_err()
        glDrawArraysInstanced(GL_TRIANGLES, 0, particle_mesh_->verts.size() * 3, N_);gl_err()
        glBindVertexArray(0);gl_err()

    }
}
