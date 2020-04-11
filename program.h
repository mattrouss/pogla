#pragma once

#include "GL/glew.h"
#include <GL/freeglut.h>
#include <string>

namespace mygl
{
class program
{
public:
    program() = default;
    ~program();

    static program* make_program(std::string &vertex_shader_src, std::string &fragment_shader_src);
    //char* get_log();
    //bool is_ready();
    void use();

    GLuint prog_id()
    {
        return prog;
    }

private:
    GLuint vert_shader;
    GLuint frag_shader;

    GLuint prog;

    GLint success_vert = GL_FALSE;
    GLint success_frag = GL_FALSE;

    GLint log_size = 0;
    GLchar *log;
};
}
