#ifndef BUMP_CAMERA_H
#define BUMP_CAMERA_H

#define cam_speed 20

#include "utils/matrix4.h"
#include "mygl/program/Program.h"
#include "mygl/movable.h"

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

    void translate(mygl::Vec3 v) override;
    void set_pos(mygl::Vec3 p) override;
    void set_pos(mygl::Vec3 v, bool update);
    mygl::Vec3 get_pos() override;

    void set_rot(mygl::Vec3 r) override;
    void rotate(mygl::Vec3 rotation) override;

    void update();

    //void rotate(float horizontal, float vertical);

    mygl::matrix4 get_view_matrix() const;

    mygl::matrix4 get_projection_matrix() const;

    void set_prog_proj(mygl::Program* p) const;
    void set_prog(mygl::Program* p);

private:
    mygl::matrix4 projection;
    mygl::matrix4 view;

    mygl::Vec3 pos;
    mygl::Vec3 left;
    mygl::Vec3 forward;

    mygl::Program* prog = nullptr;
};


#endif //BUMP_CAMERA_H
