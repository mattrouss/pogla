#pragma once

#include "GL/glew.h"
#include <GL/freeglut.h>
#include <string>
#include <vector>

namespace mygl
{
class Program
{
public:
    Program(std::vector<std::pair<GLuint, GLint>> shaders, GLuint program)
    {
        this->shaders = shaders;
        this->prog = program;
    }
    ~Program();

    void use();

    GLuint prog_id()
    {
        return prog;
    }

private:
    std::vector<std::pair<GLuint, GLint>> shaders;

    GLuint prog;
};
}
