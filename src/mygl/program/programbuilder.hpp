
#include "mygl/program/programbuilder.h"

namespace mygl
{
    template<typename SType>
    void ProgramBuilder::add_shader(SType shader)
    {
        for (auto s : shader.list())
        {
            shader_list.push_back(s);
        }
    }
}