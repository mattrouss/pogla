//
// Created by defalur on 17/06/20.
//

#include "basicmovable.h"

void BasicMovable::set_pos(mygl::Vec3 p)
{
    pos = p;
}

void BasicMovable::translate(mygl::Vec3 v)
{
    pos += v;
}

void BasicMovable::translate_local(mygl::Vec3 v)
{
    auto translation = v[0] * left + v[1] * (forward ^ left).normalized() + v[2] * forward;

    pos += translation;
}

void BasicMovable::set_rot(mygl::Vec3 r)
{
}

void BasicMovable::rotate(mygl::Vec3 r)
{
}

mygl::Vec3 BasicMovable::get_pos()
{
    return pos;
}
