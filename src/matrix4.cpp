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

    matrix4 matrix4::operator* (const matrix4& rhs)
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
    auto F = std::array<float, 3>{centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    auto UP = std::array<float, 3>{upX, upY, upZ};

    float Fmag = std::sqrt(F[0] * F[0] + F[1] * F[1] + F[2] * F[2]);
    float UPmag = std::sqrt(UP[0] * UP[0] + UP[1] * UP[1] + UP[2] * UP[2]);

    auto f = std::array<float, 3>{F[0] / Fmag, F[1] / Fmag, F[2] / Fmag};
    auto up = std::array<float, 3>{UP[0] / UPmag, UP[1] / UPmag, UP[2] / UPmag};

    //f * up
    auto S = std::array<float, 3>{
        f[1] * up[2] - f[2] * up[1],
        f[2] * up[0] - f[0] * up[2],
        f[0] * up[1] - f[1] * up[0]
    };

    float Smag = std::sqrt(S[0] * S[0] + S[1] * S[1] + S[2] * S[2]);
    auto s = std::array<float, 3>{S[0] / Smag, S[1] / Smag, S[2] / Smag};

    auto u = std::array<float, 3>{
        s[1] * f[2] - s[2] * f[1],
        s[2] * f[0] - s[0] * f[2],
        s[0] * f[1] - s[1] * f[0]
    };

    auto mat2 = mygl::matrix4{{
         S[0],  S[1],  S[2], 0,
         u[0],  u[1],  u[2], 0,
        -f[0], -f[1], -f[2], 0,
            0,     0,     0, 1,
    }};

    mat *= mat2;
}

void translate(mygl::matrix4 &mat, float x, float y, float z)
{
    auto translation = mygl::matrix4{{
        1, 0, 0, -x,
        0, 1, 0, -y,
        0, 0, 1, -z,
        0, 0, 0, 1
    }};

    mat *= translation;
}
