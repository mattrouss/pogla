#ifndef BUMP_CAMERA_H
#define BUMP_CAMERA_H


#include "matrix4.h"

class Camera
{
public:
    Camera(float left,
            float right,
            float bottom,
            float top,
            float znear,
            float zfar);

    void look_at(float x, float y, float z, float cx, float cy, float cz, float upx, float upy, float upz);

    void translate(float x, float y, float z);

    //rotate?

    mygl::matrix4 get_view_matrix() const;

    mygl::matrix4 get_projection_matrix() const;

private:
    mygl::matrix4 projection;
    mygl::matrix4 view;

    float posx;
    float posy;
    float posz;
};


#endif //BUMP_CAMERA_H
