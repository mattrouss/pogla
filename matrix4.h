#pragma once

#include <array>
#include <iostream>

namespace mygl
{
class matrix4
{
public:
    matrix4();
    matrix4(std::array<float, 16> data_);
    matrix4 operator* (const matrix4& rhs);
    void operator*= (const matrix4& rhs);
    static matrix4 identity();

    friend std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m);

public:
    std::array<float, 16> data;
};

std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m);
}

void frustum(mygl::matrix4 &mat,
        const float &left, const float &right,
        const float &bottom, const float &top,
        const float &z_near, const float &z_far);

void look_at(mygl::matrix4 &mat,
        const float &eyeX, const float &eyeY, const float &eyeZ,
        const float &centerX, const float &centerY, const float &centerZ,
        float upX, float upY, float upZ);

void translate(mygl::matrix4 &mat, float x, float y, float z);
