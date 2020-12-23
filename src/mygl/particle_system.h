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
        Particle(const mygl::Vec3& pos): pos_(pos) {}

        mygl::Vec3 pos_;

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

        void render();
        void init_particles();

    private:
        mygl::Program* display_prog_;
        mygl::Program* compute_prog_;
        mygl::Program* sort_prog_;
        GLuint vao_;

        size_t N_;
        size_t N_x_;
        size_t N_y_;
        std::shared_ptr<mygl::mesh> particle_mesh_;

        std::vector<mygl::Vec3> positions_a_;
        std::vector<mygl::Vec3> positions_b_;
        unsigned iteration_parity = 0;

    };
}

#endif //PARTICLE_SYSTEM_H
