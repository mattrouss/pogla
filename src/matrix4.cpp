#include "matrix4.h"
#include <cmath>

namespace mygl
{
    matrix4::matrix4()
    {
        data = {1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1};
    }

    matrix4::matrix4(std::array<float, 16> data_)
    {
        data = data_;
    }

    matrix4 matrix4::identity()
    {
        return matrix4{{1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1}};
    }

    void matrix4::operator*= (const matrix4& rhs)
    {
        auto tmp = std::array<float, 16>{};
        for (unsigned i = 0; i < 4; i++)
        {
            for (unsigned j = 0; j < 4; j++)
            {
                float sum = 0.0;
                for (unsigned k = 0; k < 4; k++)
                {
                    sum += data[i * 4 + k] * rhs.data[k * 4 + j];
                }
                tmp[i * 4 + j] = sum;
            }
        }

        data = tmp;
    }

    matrix4 matrix4::operator*(const matrix4& rhs)
    {
        auto result = matrix4{data};
        result *= rhs;
        return result;
    }

    std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m)
    {
        out << "(";
        for (unsigned i = 0; i < 4; i++)
        {
            for (unsigned j = 0 ; j < 4; j++)
            {
                out << " " << m.data[i * 4 + j];
            }
            out << "\n ";
        }
        return out;
    }

    matrix4 matrix4::transpose() const {
        matrix4 t;
        for (unsigned i = 0; i < 4u; i++)
            for (unsigned j = 0; j < 4u; j++)
                t.data[i * 4 + j] = this->data[j * 4 + i];

        return t;
    }

    void matrix4::translate(const mygl::Vec3 &v)
    {
        auto translation = mygl::matrix4{{
                                                 1, 0, 0, v[0],
                                                 0, 1, 0, v[1],
                                                 0, 0, 1, v[2],
                                                 0, 0, 0, 1
                                         }};

        *this *= translation;
    }

    matrix4 rot_y(float angle)
    {
        float cos_a = cosf(angle);
        float sin_a = sinf(angle);
        return mygl::matrix4{{
            cos_a, 0, sin_a, 0,
                 0, 1,     0, 0,
            -sin_a, 0, cos_a, 0,
                 0, 0,     0, 1
        }};
    }

    matrix4 rot_z(float angle)
    {
        float cos_a = cosf(angle);
        float sin_a = sinf(angle);
        return mygl::matrix4{{
            cos_a, -sin_a, 0, 0,
            sin_a,  cos_a, 0, 0,
                0,      0, 1, 0,
                0,      0, 0, 1
        }};
    }

    matrix4 rot_x(float angle)
    {
        float cos_a = cosf(angle);
        float sin_a = sinf(angle);
        return mygl::matrix4{{
            1,     0,      0, 0,
            0, cos_a, -sin_a, 0,
            0, sin_a,  cos_a, 0,
            0,     0,      0, 1
        }};
    }

    void matrix4::rotate(const mygl::Vec3 &v)
    {
        //x: pitch
        //y: yaw
        //z: roll

        //order: yaw pitch roll
        auto pitch = rot_x(v[0]);
        auto yaw = rot_y(v[1]);
        auto roll = rot_z(v[2]);

        auto rot = yaw * pitch * roll;

        *this *= rot;//.transpose();//not sure about the transpose, needs testing
    }

    void matrix4::set_rot(const mygl::Vec3 &v)
    {
        this->data = {
                1,0,0,data[3],
                0,1,0,data[7],
                0,0,1,data[11],
                0,0,0,1
                };
        this->rotate(v);
    }

    void matrix4::set_pos(const mygl::Vec3 &v)
    {
        this->data[3] = 0;
        this->data[7] = 0;
        this->data[11] = 0;

        this->translate(v);
    }

    mygl::Vec3 matrix4::get_pos()
    {
        return mygl::Vec3{{this->data[3], this->data[7], this->data[11]}};
    }

    mygl::Vec4 operator*(const mygl::matrix4& lhs, const mygl::Vec4& rhs) {
        auto v = mygl::Vec4{};
        for (unsigned i = 0; i < 4u; i++) {
            float sum = 0;
            for (unsigned j = 0; j < 4u; j++)
                sum += lhs.data[i * 4 + j] * rhs[j];
            v[i] = sum;
        }
        return v;
    }
}

void frustum(mygl::matrix4 &mat,
        const float &left, const float &right,
        const float &bottom, const float &top,
        const float &z_near, const float &z_far)
{
    float k1 = (2 * z_near) / (right - left);
    float k2 = (2 * z_near) / (top - bottom);

    float A = (right + left) / (right - left);
    float B = (top + bottom) / (top - bottom);
    float C = -(z_far + z_near) / (z_far - z_near);
    float D = -(2 * z_far * z_near) / (z_far - z_near);

    auto mat2 = mygl::matrix4{
        {k1, 0,  A, 0,
                0,  k2, B, 0,
                0,  0,  C, D,
                0,  0, -1, 0}};

    mat *= mat2;
}

void look_at(mygl::matrix4 &mat, const mygl::Vec3& eye, const mygl::Vec3& target, const mygl::Vec3& up)
{
    auto f = (target - eye).normalized();
    auto l = (up ^ f).normalized();
    auto u = f ^ l;

    auto mat2 = mygl::matrix4{{
         l[0],  l[1],  l[2], 0,
         u[0],  u[1],  u[2], 0,
        -f[0], -f[1], -f[2], 0,
            0,     0,     0, 1,
    }};

    mat *= mat2;
}

void translate(mygl::matrix4 &mat, const mygl::Vec3& v)
{
    auto translation = mygl::matrix4{{
        1, 0, 0, v[0],
        0, 1, 0, v[1],
        0, 0, 1, v[2],
        0, 0, 0, 1
    }};

    mat *= translation;
}
