#ifndef BUMP_PROGRAMBUILDER_H
#define BUMP_PROGRAMBUILDER_H

#include <vector>
#include <memory>
#include <optional>

#include "mygl/program/Program.h"

#include "GL/glew.h"
#include <GL/freeglut.h>

namespace mygl
{
    class ProgramBuilder
    {
    public:
        //using templates purely for training purposes
        template<typename SType>
        void add_shader(SType shader);

        std::optional<std::shared_ptr<Program>> build();

    private:
        std::vector<std::pair<GLuint, GLint>> source_shaders() const;
        inline void handle_error(std::vector<std::pair<GLuint, GLint>> shaders,
                                 GLuint failed,
                                 std::string failed_src) const;
        inline void handle_link_error(std::vector<std::pair<GLuint, GLint>> shaders,
                GLuint program);

    private:
        std::vector<std::pair<std::string, GLenum>> shader_list;
    };
}

#include "programbuilder.hpp"

#endif //BUMP_PROGRAMBUILDER_H
