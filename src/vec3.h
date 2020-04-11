//
// Created by mattrouss on 4/11/20.
//

#pragma once

#pragma once

#include <ostream>
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace mygl {
    class Vec3
    {
    public:
        Vec3(const std::array<float, 3>& data): data_(data) {}

        float operator[](size_t idx) const;
        float& operator[](size_t idx);

        Vec3& operator+=(const Vec3 &v);
        Vec3& operator-=(const Vec3 &v);
        Vec3& operator*=(const float &l);
        Vec3& operator/=(const float &l);

        Vec3& operator^=(const Vec3& rhs);

        friend Vec3 operator*(const float &l, Vec3 v);
        friend Vec3 operator*(Vec3 v, const float &l);
        friend Vec3 operator/(const float &l, Vec3 v);
        friend Vec3 operator/(Vec3 v, const float &l);

        friend std::ostream& operator<<(std::ostream &out, const Vec3 &vec);

        float sqr_magnitude() const;
        float magnitude() const;
        Vec3 normalized() const;

    private:
        std::array<float, 3> data_;
    };

    inline Vec3 operator+(Vec3 lhs, const Vec3 &rhs) {
        lhs += rhs;
        return lhs;
    }

    inline Vec3 operator-(Vec3 lhs, const Vec3 &rhs) {
        lhs -= rhs;
        return lhs;
    }

    inline float operator*(const Vec3 &lhs, const Vec3 &rhs) {
        float acc = 0;
        for (unsigned i = 0; i < 3; i++)
            acc += lhs[i] * rhs[i];
        return acc;
    }


    inline Vec3 operator^(Vec3 lhs, Vec3 const& rhs) {
        lhs ^= rhs;
        return lhs;
    }


}
