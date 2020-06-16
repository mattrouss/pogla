#ifndef BUMP_BASICMOVABLE_H
#define BUMP_BASICMOVABLE_H


#include "movable.h"
#include "vector.h"

class BasicMovable : public Movable
{
public:
    BasicMovable() = default;

    void set_pos(mygl::Vec3 p) override;

    void translate(mygl::Vec3 v) override;

    void set_rot(mygl::Vec3 r) override;

    void rotate(mygl::Vec3 r) override;

    mygl::Vec3 get_pos() override;

    mygl::Vec3 pos;
};


#endif //BUMP_BASICMOVABLE_H
