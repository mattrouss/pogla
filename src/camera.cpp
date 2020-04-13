#include "camera.h"

Camera::Camera(const float left, const float right, const float bottom, const float top, const float znear,
               const float zfar)
{
    projection = mygl::matrix4::identity();
    frustum(projection, left, right, bottom, top, znear, zfar);
}

void Camera::look_at(const mygl::Vec3& eye, const mygl::Vec3& target, const mygl::Vec3& up)
{
    view = mygl::matrix4::identity();
    ::look_at(view, eye, target, up);
    translate(eye);
}

void Camera::translate(const mygl::Vec3& v)
{
    ::translate(view, -1 * v);
    pos = v;
}

mygl::matrix4 Camera::get_view_matrix() const
{
    return view;
}

mygl::matrix4 Camera::get_projection_matrix() const
{
    return projection;
}
