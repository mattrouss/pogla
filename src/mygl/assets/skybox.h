#ifndef SKYBOX_H
#define SKYBOX_H

#include "mygl/program/Program.h"
#include "assets/mesh.h"
#include "mygl/texturemanager.h"

#include <string>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include "mygl/gl_err.h"

namespace mygl {
    class Skybox
    {
    public:
        Skybox() = default;

        void init_skybox(Program* prog, const std::string& cubemap_path);
        void render(float deltatime);

    private:
        GLuint vao_;
        mygl::Program* prog_;

        std::vector<mygl::Vec3> verts_;
        TextureManager texture_manager_;
    };
}

#endif //SKYBOX_H
