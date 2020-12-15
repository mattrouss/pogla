#include <iostream>
#include <fstream>
#include <sstream>
#include "Program.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

namespace mygl
{
    void Program::use()
    {
        glUseProgram(prog);
        auto err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
    }

    Program::~Program()
    {
        glDeleteProgram(prog);
        for (auto shader: shaders)
        {
            if (shader.second == GL_TRUE)
            {
                glDeleteShader(shader.first);
            }
        }
    }
}
