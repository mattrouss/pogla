#pragma once

#include <array>
#include <iostream>

#include "vector.h"

namespace mygl
{
class matrix4
{
public:
    matrix4();
    matrix4(std::array<float, 16> data_);
    matrix4 operator* (const matrix4& rhs);
    void operator*= (const matrix4& rhs);

    matrix4 transpose() const;
    static matrix4 identity();

    friend std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m);

    void translate(const mygl::Vec3& v);
    void set_pos(const mygl::Vec3& v);
    mygl::Vec3 get_pos();
    void rotate(const mygl::Vec3& v);
    void set_rot(const mygl::Vec3& v);

public:
    std::array<float, 16> data;
};

mygl::Vec4 operator*(const mygl::matrix4& lhs, const mygl::Vec4& rhs);

std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m);
}

void frustum(mygl::matrix4 &mat,
        const float &left, const float &right,
        const float &bottom, const float &top,
        const float &z_near, const float &z_far);

void look_at(mygl::matrix4 &mat,
        const mygl::Vec3& eye, const mygl::Vec3& target, const mygl::Vec3& up);

void translate(mygl::matrix4 &mat, const mygl::Vec3& v);
