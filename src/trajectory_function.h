#ifndef BUMP_TRAJECTORY_FUNCTION_H
#define BUMP_TRAJECTORY_FUNCTION_H

#include "vector.h"

namespace TFunc
{
    enum : unsigned int
    {
        DELTA_TIME = 1,//function takes delta time
        ABS_TIME = 2,//function takes time since start (default and takes priority)
        TRANSLATE = 4,//function result is used to translate objects
        SET_POS = 8,//function result is used to set position of objects directly (default and takes priority)
        DELTA_POS = 16,//function result is a translation
        ABS_POS = 32//function result is the position of the object (default)
    };
}

struct TrajectoryFunction
{
    std::function<mygl::Vec3(float t)> func;
    uint8_t flags = 0;

    inline mygl::Vec3 operator()(float t)
    {
        return func(t);
    }

    //FIXME: Fix getters to have priorities and default values
    bool delta_time()
    {
        return flags & TFunc::DELTA_TIME;
    }

    bool abs_time()
    {
        return flags & TFunc::ABS_TIME;
    }

    bool translate()
    {
        return flags & TFunc::TRANSLATE;
    }

    bool set_pos()
    {
        return flags & TFunc::SET_POS;
    }

    bool delta_pos()
    {
        return flags & TFunc::DELTA_POS;
    }

    bool abs_pos()
    {
        return flags & TFunc::ABS_POS;
    }
};

#endif //BUMP_TRAJECTORY_FUNCTION_H
