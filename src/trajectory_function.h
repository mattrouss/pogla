#ifndef BUMP_TRAJECTORY_FUNCTION_H
#define BUMP_TRAJECTORY_FUNCTION_H

#include "utils/vector.h"

namespace TFunc
{
    enum : unsigned int
    {
        DELTA_TIME = 1,//function takes delta time
        ABS_TIME = 2,//function takes time since start (default and takes priority)
        TRANSLATE = 4,//function result is used to translate objects
        SET_POS = 8,//function result is used to set position of objects directly (default and takes priority)
        DELTA_POS = 16,//function result is a translation
        ABS_POS = 32,//function result is the position of the object (default)
        USE_POSITION = 64,//function result will affect object position
        USE_ROTATION = 128//function result will affect object rotation
    };
}

//TRANSLATE, SET_POS, DELTA_POS, ABS_POS also apply to rotations

struct TrajectoryFunction
{
    using ReturnType = std::pair<mygl::Vec3, mygl::Vec3>;
    using FType = std::function<ReturnType>;
    std::function<ReturnType (float t)> func;
    unsigned flags = 0;

    inline ReturnType operator()(float t)
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

    bool use_position()
    {
        return flags & TFunc::USE_POSITION;
    }

    bool use_rotation()
    {
        return flags & TFunc::USE_ROTATION;
    }
};

#endif //BUMP_TRAJECTORY_FUNCTION_H
