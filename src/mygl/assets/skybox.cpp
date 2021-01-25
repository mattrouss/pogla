#include "skybox.h"

namespace mygl
{
    void Skybox::init_skybox(Program* prog, const std::string& cubemap_path)
    {
        prog_ = prog;
        texture_manager_ = TextureManager();
        texture_manager_.load_cubemap(cubemap_path);
        prog_->use();

        float skybox_size = 200.f;
        verts_ = std::vector<mygl::Vec3>{
            skybox_size * Vec3{{ -1.0f,  1.0f, -1.0f }},
            skybox_size * Vec3{{ -1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f, -1.0f }},
            skybox_size * Vec3{{ -1.0f,  1.0f, -1.0f }},

            skybox_size * Vec3{{ -1.0f, -1.0f,  1.0f }},
            skybox_size * Vec3{{ -1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{ -1.0f,  1.0f, -1.0f }},
            skybox_size * Vec3{{ -1.0f,  1.0f, -1.0f }},
            skybox_size * Vec3{{ -1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{ -1.0f, -1.0f,  1.0f }},

            skybox_size * Vec3{{  1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f, -1.0f }},

            skybox_size * Vec3{{ -1.0f, -1.0f,  1.0f }},
            skybox_size * Vec3{{ -1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f,  1.0f }},
            skybox_size * Vec3{{ -1.0f, -1.0f,  1.0f }},

            skybox_size * Vec3{{ -1.0f,  1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{ -1.0f,  1.0f,  1.0f }},
            skybox_size * Vec3{{ -1.0f,  1.0f, -1.0f }},

            skybox_size * Vec3{{ -1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{ -1.0f, -1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f, -1.0f }},
            skybox_size * Vec3{{ -1.0f, -1.0f,  1.0f }},
            skybox_size * Vec3{{  1.0f, -1.0f,  1.0f }}
        };
        
        glGenVertexArrays(1, &vao_);gl_err()
        glBindVertexArray(vao_);gl_err()

        // Create mesh data vbo
        GLuint vertex_buffer_id;
        glGenBuffers(1, &vertex_buffer_id);gl_err()

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);gl_err();
        glBufferData(GL_ARRAY_BUFFER, verts_.size() * sizeof(mygl::Vec3), verts_.data(), GL_STATIC_DRAW);gl_err();

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);gl_err();
        //positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vec3), (void *)0);gl_err();
        glEnableVertexAttribArray(0);gl_err();

    }

    void Skybox::render(float deltatime)
    {
        glDepthMask(GL_FALSE);
        prog_->use();

        glBindVertexArray(vao_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_manager_.get(0));gl_err();
        glDrawArrays(GL_TRIANGLES, 0, 36);gl_err();
        glDepthMask(GL_TRUE);

    }
}
