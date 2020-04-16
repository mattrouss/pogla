#include "camera.h"
#include "gl_err.h"

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
    if (t)
        translate(eye, false);
}

void Camera::translate(const mygl::Vec3 v)
{//translate in local base
    std::cout << "Translating\n";
    mygl::Vec3 left_h = {{left[0], 0, left[2]}};
    mygl::Vec3 forward_h = {{forward[0], 0, forward_h[3]}};

    auto translation = v[0] * left_h + v[1] * mygl::Vec3{{0.0,1.0,0.0}} + v[2] * forward_h;
    translate(translation, true);

    set_prog_proj(prog);
}

void Camera::translate(const mygl::Vec3 v, bool update)
{
    ::translate(view, -1 * v);
    pos = v;
    if (update)//a bit dirty but does the trick for now
        look_at(pos, {{0,0,0}}, {{0,1,0}}, false);
}

mygl::matrix4 Camera::get_view_matrix() const
{
    return view;
}

mygl::matrix4 Camera::get_projection_matrix() const
{
    return projection;
}

void Camera::set_prog_proj(mygl::program *p) const
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

void Camera::set_prog(mygl::program* p)
{
    prog = p;
}
