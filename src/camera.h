#ifndef BUMP_CAMERA_H
#define BUMP_CAMERA_H

#define cam_speed 20

#include "matrix4.h"
#include "program.h"
#include "movable.h"

class Camera : public Movable
{
public:
    Camera(float left,
            float right,
            float bottom,
            float top,
            float znear,
            float zfar);

    void look_at(const mygl::Vec3& eye, const mygl::Vec3& target, const mygl::Vec3& up,
            bool translate = true);

    void translate(const mygl::Vec3 v) override;
    void set_pos(const mygl::Vec3 v, bool update);

    void rotate(const mygl::Vec3 rotation) override;

    //void rotate(float horizontal, float vertical);

    mygl::matrix4 get_view_matrix() const;

    mygl::matrix4 get_projection_matrix() const;

    void set_prog_proj(mygl::program* p) const;
    void set_prog(mygl::program* p);

private:
    mygl::matrix4 projection;
    mygl::matrix4 view;

    mygl::Vec3 pos;
    mygl::Vec3 left;
    mygl::Vec3 forward;

    mygl::program* prog = nullptr;
};


#endif //BUMP_CAMERA_H
