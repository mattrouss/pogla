#ifndef BUMP_SHADERTYPES_H
#define BUMP_SHADERTYPES_H

#include "GL/glew.h"
#include <GL/freeglut.h>

#include <string>
#include <vector>

namespace mygl::shaders
{

    struct DisplayShadersBase
    {
    public:
        DisplayShadersBase(std::string vertex_src, std::string fragment_src)
        {
            this->vertex_src = vertex_src;
            this->fragment_src = fragment_src;
        }

        std::vector<std::pair<std::string, GLenum>> list() const
        {
            return {std::make_pair(vertex_src, GL_VERTEX_SHADER),
                    std::make_pair(fragment_src, GL_FRAGMENT_SHADER)};
        }

    private:
        std::string vertex_src;
        std::string fragment_src;
    };

    struct ComputeShader
    {
    public:
        ComputeShader(std::string src)
        {
            this->src = src;
        }

        std::vector<std::pair<std::string, GLenum>> list() const
        {
            return {std::make_pair(src, GL_COMPUTE_SHADER)};
        }

    private:
        std::string src;
    };
}
#endif //BUMP_SHADERTYPES_H
