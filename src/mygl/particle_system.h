#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <cstddef>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include "mygl/gl_err.h"

#include "mygl/program/Program.h"
#include "utils/vector.h"
#include "assets/mesh.h"

namespace mygl
{
    struct Particle
    {
    public:
        Particle() = default;
        Particle(const mygl::matrix4& transform): transform_(transform) {}
        Particle(const mygl::matrix4& tranform, const mygl::Vec3& velocity): transform_(tranform), velocity_(velocity) {}

        mygl::matrix4 transform_;
        mygl::Vec3 velocity_;
        float angle;

        mygl::Vec3 pos() const;

    };

    class ParticleSystem
    {
    public:
        ParticleSystem(size_t N);

        void init_system(mygl::Program* display_prog, mygl::Program* compute_prog, std::shared_ptr<mygl::mesh> mesh);
        void init_system(mygl::Program* display_prog,
                mygl::Program* compute_prog,
                mygl::Program* sort_prog,
                std::shared_ptr<mygl::mesh> mesh);

        void render(float deltatime);
        void init_particles();

    private:
        void retrieve_ssbo();
        void print_buffer(std::string name, std::vector<mygl::Particle> buffer);
        bool check_sorted(std::vector<mygl::Particle> buffer);
        void print_sorted(std::string name, std::vector<mygl::Particle> buffer);
        void print_ssbo();

    private:
        mygl::Program* display_prog_;
        mygl::Program* compute_prog_;
        mygl::Program* sort_prog_;
        GLuint vao_;

        size_t N_;
        size_t N_x_;
        size_t N_y_;
        size_t N_z_;
        std::shared_ptr<mygl::mesh> particle_mesh_;

        std::vector<mygl::Particle> particles_a_;
        GLuint ssbo_a_;
        std::vector<mygl::Particle> particles_b_;
        GLuint ssbo_b_;
        unsigned iteration_parity = 0;

    };
}

#endif //PARTICLE_SYSTEM_H
