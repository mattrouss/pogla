#include "matrix4.h"
#include <cmath>

namespace mygl
{
    matrix4::matrix4()
    {
        data.fill(0);
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
        for (auto i = 0; i < 4u; i++)
            for (auto j = 0; j < 4u; j++)
                t.data[i * 4 + j] = this->data[j * 4 + i];

        return t;
    }

    mygl::Vec4 operator*(const mygl::matrix4& lhs, const mygl::Vec4& rhs) {
        auto v = mygl::Vec4{};
        for (auto i = 0; i < 4u; i++) {
            float sum = 0;
            for (auto j = 0; j < 4u; j++)
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

void look_at(mygl::matrix4 &mat,
        const float &eyeX, const float &eyeY, const float &eyeZ,
        const float &centerX, const float &centerY, const float &centerZ,
        float upX, float upY, float upZ)
{
    auto F = mygl::Vec3({centerX - eyeX, centerY - eyeY, centerZ - eyeZ});
    auto up = mygl::Vec3({upX, upY, upZ});
    F = F.normalized();
    up = up.normalized();

    auto s = up ^ F;
    auto u = F ^ s.normalized();

    std::cout << "f: " << F << " s: " << s << " u: " << u << "\n";

    auto mat2 = mygl::matrix4{{
         s[0],  s[1],  s[2], 0,
         u[0],  u[1],  u[2], 0,
        -F[0], -F[1], -F[2], 0,
            0,     0,     0, 1,
    }};

    mat *= mat2;
}

void translate(mygl::matrix4 &mat, float x, float y, float z)
{
    auto translation = mygl::matrix4{{
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    }};

    mat *= translation;
}
