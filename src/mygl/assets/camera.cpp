#include "camera.h"
#include "mygl/gl_err.h"
#include "utils/clock.h"

Camera::Camera(const float left, const float right, const float bottom, const float top, const float znear,
               const float zfar)
{
    projection = mygl::matrix4::identity();
    frustum(projection, left, right, bottom, top, znear, zfar);
}

void Camera::look_at(const mygl::Vec3& eye, const mygl::Vec3& target, const mygl::Vec3& up,
        bool t)
{
    view = mygl::matrix4::identity();
    ::look_at(view, eye, target, up);
    forward = (target - eye).normalized();
    left = up.normalized() ^ forward;
    if (t)
        set_pos(eye, false);
}

void Camera::translate(const mygl::Vec3 v)
{//translate in local base
    mygl::Vec3 left_h = {{left[0], 0, left[2]}};
    mygl::Vec3 forward_h = {{forward[0], 0, forward[2]}};

    auto translation = v[0] * left_h + v[1] * mygl::Vec3{{0.0,1.0,0.0}} + v[2] * forward_h;

    set_pos(pos + (translation * cam_speed * mainClock.deltatime()), true);

    set_prog_proj(prog);
}

void Camera::set_pos(const mygl::Vec3 v, bool update)
{
    ::translate(view, -1 * v);
    pos = v;
    if (update)//a bit dirty but does the trick for now
        look_at(pos, {{0,0,0}}, {{0,1,0}}, true);
    set_prog_proj(prog);
}

mygl::matrix4 Camera::get_view_matrix() const
{
    return view;
}

mygl::matrix4 Camera::get_projection_matrix() const
{
    return projection;
}

void Camera::set_prog_proj(mygl::Program *p) const
{
    GLint mat_proj_id;
    GLint mat_obj_id;

    mat_proj_id = glGetUniformLocation(p->prog_id(), "projection_matrix");gl_err();
    mat_obj_id = glGetUniformLocation(p->prog_id(), "view_matrix");gl_err();
    glUniformMatrix4fv(mat_proj_id, 1, GL_FALSE, projection.transpose().data.data());gl_err();
    glUniformMatrix4fv(mat_obj_id, 1, GL_FALSE, view.transpose().data.data());gl_err();
}

void Camera::rotate(mygl::Vec3 rotation)
{
    this->projection.rotate(rotation);
}

void Camera::set_prog(mygl::Program* p)
{
    prog = p;
}

void Camera::set_pos(const mygl::Vec3 p)
{
    set_pos(p, false);
}

void Camera::set_rot(mygl::Vec3 r)
{
    r = -1 * r; //on the camera we need to invert the view matrix
    projection.set_rot(r);
    set_prog_proj(prog);
}

void Camera::update()
{
    set_prog_proj(prog);
}

mygl::Vec3 Camera::get_pos()
{
    return pos;
}
