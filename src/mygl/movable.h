#ifndef BUMP_MOVABLE_H
#define BUMP_MOVABLE_H

#include "utils/vector.h"

class Movable
{
public:
    virtual void set_pos(mygl::Vec3 p) = 0;
    virtual void translate(mygl::Vec3 v) = 0;
    virtual void set_rot(mygl::Vec3 r) = 0;
    virtual void rotate(mygl::Vec3 r) = 0;
    virtual mygl::Vec3 get_pos() = 0;
};

#endif //BUMP_MOVABLE_H
