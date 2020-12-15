#include "programbuilder.h"
#include "gl_err.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace mygl
{
    std::vector<std::pair<GLuint, GLint>> ProgramBuilder::source_shaders() const
    {
        std::vector<std::pair<GLuint, GLint>> result;
        for (auto shader_data: shader_list)
        {
            std::ifstream file_in(shader_data.first);
            std::stringstream source_stream;
            source_stream << file_in.rdbuf();
            file_in.close();

            char* shader_src = new char[source_stream.str().length()];
            source_stream.str().copy(shader_src, source_stream.str().length());
            const int f_len = source_stream.str().length();

            auto shader = glCreateShader(shader_data.second);gl_err();
            glShaderSource(shader, 1, &shader_src, &f_len);gl_err();
            glCompileShader(shader);gl_err();

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            gl_err();

            result.emplace_back(shader, success);
            if (success == GL_FALSE)
            {
                handle_error(result, shader, shader_data.first);
            }
            delete[](shader_src);
        }
        return result;
    }

    std::optional<std::shared_ptr<Program>> ProgramBuilder::build()
    {
        auto shaders = source_shaders();
        if (std::any_of(shaders.begin(), shaders.end(),
                [](std::pair<GLuint, GLint> x){
            return x.second == GL_FALSE;
        }))
        {
            return std::nullopt;
        }

        auto program = glCreateProgram();gl_err();
        for (auto shader: shaders)
        {
            glAttachShader(program, shader.first);gl_err();
        }


        glLinkProgram(program);gl_err();

        GLint linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        gl_err();
        if (linked == GL_FALSE)
        {
            handle_link_error(shaders, program);
            return std::nullopt;
        }

        for (auto shader: shaders)
        {
            glDetachShader(program, shader.first);
            gl_err();
        }

        return std::optional<std::shared_ptr<Program>>{
            std::make_shared<Program>(shaders, program)
        };
    }

    void ProgramBuilder::handle_link_error(std::vector<std::pair<GLuint, GLint>> shaders,
                                           GLuint program)
    {
        GLint log_size = 0;
        GLchar *log;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
        gl_err();

        log = new GLchar[log_size];
        glGetProgramInfoLog(program, log_size, &log_size, log);
        gl_err();

        std::cout << "Link:\n" << log << "\n";

        glDeleteProgram(program);
        gl_err();

        for (auto shader: shaders)
        {
            glDeleteShader(shader.first);
            gl_err();
        }
    }

    void ProgramBuilder::handle_error(
            std::vector<std::pair<GLuint, GLint>> shaders,
            GLuint failed,
            std::string failed_src) const
    {
        GLint log_size = 0;
        GLchar *log;
        glGetShaderiv(failed, GL_INFO_LOG_LENGTH, &log_size);
        gl_err();
        log = new GLchar[log_size];
        glGetShaderInfoLog(failed, log_size, &log_size, log);
        gl_err();
        std::cout << failed_src << ":\n" << log << "\n";
        for (auto shader : shaders)
        {
            glDeleteShader(shader.first);
            gl_err();
        }
        delete log;
    }
}
