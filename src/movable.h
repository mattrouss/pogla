#ifndef BUMP_MOVABLE_H
#define BUMP_MOVABLE_H

class Movable
{
public:
    virtual void translate(const mygl::Vec3 v) = 0;
    virtual void rotate(mygl::Vec3 r) = 0;
};

#endif //BUMP_MOVABLE_H
