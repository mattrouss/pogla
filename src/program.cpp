#include <iostream>
#include <fstream>
#include <sstream>
#include "program.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

namespace mygl
{
    program* program::make_program(std::string &vertex_chader_src, std::string &fragment_shader_src)
    {
        std::ifstream f_v(vertex_chader_src);
        std::stringstream s_v;
        s_v << f_v.rdbuf();
        f_v.close();

        std::ifstream f_f(fragment_shader_src);
        std::stringstream s_f;
        s_f << f_f.rdbuf();
        f_f.close();

        char* v_shader_src = new char[s_v.str().length()];
        s_v.str().copy(v_shader_src, s_v.str().length());
        const int v_len = s_v.str().length();
        char* f_shader_src = new char[s_f.str().length()];
        s_f.str().copy(f_shader_src, s_f.str().length());
        const int f_len = s_f.str().length();

        auto result = new program{};

        auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &v_shader_src, &v_len);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cout << "OpenGL ERROR: " << __LINE__ << std::endl;
        }

        glCompileShader(vert_shader);
        err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cout << "OpenGL ERROR: " << __LINE__ << std::endl;
        }

        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result->success_vert);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        if (result->success_vert == GL_FALSE)
        {
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &result->log_size);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            result->log = new GLchar[result->log_size];
            glGetShaderInfoLog(vert_shader, result->log_size, &result->log_size, result->log);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            std::cout << "Vert:\n" << result->log << "\n";
            glDeleteShader(vert_shader);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            delete result;
            return nullptr;
        }

        auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        glShaderSource(frag_shader, 1, &f_shader_src, &f_len);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        glCompileShader(frag_shader);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result->success_frag);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        if (result->success_frag == GL_FALSE)
        {
            glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &result->log_size);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            result->log = new GLchar[result->log_size];
            glGetShaderInfoLog(frag_shader, result->log_size, &result->log_size, result->log);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            std::cout << "Fragment:\n" << result->log << "\n";
            glDeleteShader(vert_shader);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
            glDeleteShader(frag_shader);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            delete result;
            return nullptr;
        }

        auto program = glCreateProgram();
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        glAttachShader(program, vert_shader);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        glAttachShader(program, frag_shader);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

        glLinkProgram(program);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

        GLint linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        if (linked == GL_FALSE)
        {
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result->log_size);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            result->log = new GLchar[result->log_size];
            glGetProgramInfoLog(program, result->log_size, &result->log_size, result->log);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            glDeleteProgram(program);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            glDeleteShader(vert_shader);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
            glDeleteShader(frag_shader);
            err = glGetError();
            if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

            delete result;
            return nullptr;
        }

        result->vert_shader = vert_shader;
        result->frag_shader = frag_shader;
        result->prog = program;

        glDetachShader(program, vert_shader);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
        glDetachShader(program, frag_shader);
        err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;

        return result;
    }

    void program::use()
    {
        glUseProgram(prog);
        auto err = glGetError();
        if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;
    }

    program::~program()
    {
        if (success_vert == GL_TRUE and success_frag == GL_TRUE)
        {
            glDeleteProgram(prog);
            glDeleteShader(vert_shader);
            glDeleteShader(frag_shader);
        }
    }
}
