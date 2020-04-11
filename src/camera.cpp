#include "camera.h"

Camera::Camera(const float left, const float right, const float bottom, const float top, const float znear,
               const float zfar)
{
    projection = mygl::matrix4::identity();
    frustum(projection, left, right, bottom, top, znear, zfar);
    posx = posy = posz = 0;
}

void Camera::look_at(float x, float y, float z, float cx, float cy, float cz, float upx, float upy, float upz)
{
    view = mygl::matrix4::identity();
    ::look_at(view, x, y, z, cx, cy, cz, upx, upy, upz);
    translate(x, y, z);
}

void Camera::translate(float x, float y, float z)
{
    ::translate(view, -x, -y, -z);
    posx = x;
    posy = y;
    posz = z;
}

mygl::matrix4 Camera::get_view_matrix() const
{
    return view;
}

mygl::matrix4 Camera::get_projection_matrix() const
{
    return projection;
}
