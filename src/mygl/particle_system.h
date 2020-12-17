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


class ParticleSystem
{
public:
    ParticleSystem(size_t N);

    void init_system(std::shared_ptr<mygl::mesh> mesh);

    void render() const;
    void init_particles();

private:
    mygl::Program* prog_;
    GLuint vao_;

    size_t N_;
    std::shared_ptr<mygl::mesh> particle_mesh_;

    std::vector<mygl::Vec3> positions_;

};

#endif //PARTICLE_SYSTEM_H
