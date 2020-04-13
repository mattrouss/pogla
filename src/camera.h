#ifndef BUMP_CAMERA_H
#define BUMP_CAMERA_H


#include "matrix4.h"
#include "program.h"

class Camera
{
public:
    Camera(float left,
            float right,
            float bottom,
            float top,
            float znear,
            float zfar);

    void look_at(const mygl::Vec3& eye, const mygl::Vec3& target, const mygl::Vec3& up);

    void translate(const mygl::Vec3& v);

    //rotate?

    mygl::matrix4 get_view_matrix() const;

    mygl::matrix4 get_projection_matrix() const;

    void set_prog_proj(mygl::program* prog) const;

private:
    mygl::matrix4 projection;
    mygl::matrix4 view;

    mygl::Vec3 pos;
};


#endif //BUMP_CAMERA_H
