//
// Created by mattrouss on 4/11/20.
//

#pragma once

#pragma once

#include <ostream>
#include <cstdint>
#include <cmath>
#include <algorithm>

#include "yaml-cpp/yaml.h"

namespace mygl {
    template <typename T, size_t D>
    class Vector
    {
    public:
        using Ts = std::array<T, D>;

        Vector(): data_{} {}
        Vector(const Ts& data): data_(data) {}

        float operator[](size_t idx) const {
            return data_[idx];
        }

        float& operator[](size_t idx) {
            return data_[idx];
        }

        Vector& operator+=(const Vector &v) {
            for (unsigned i = 0; i < D; i++)
                data_[i] += v[i];

            return *this;
        }
        Vector& operator-=(const Vector &v) {
            for (unsigned i = 0; i < D; i++)
                data_[i] -= v[i];

            return *this;
        }
        Vector& operator*=(const float &l) {
            for (unsigned i = 0; i < D; i++)
                data_[i] *= l;

            return *this;
        }
        Vector& operator/=(const float &l) {
            for (unsigned i = 0; i < D; i++)
                data_[i] /= l;

            return *this;
        }

        Vector& operator^=(const Vector& rhs) {
            (void) rhs;
            throw "Operator not implemented for dimension " + std::string(D);
        }

        float sqr_magnitude() const {
            return (*this) * (*this);
        }

        float magnitude() const {
            return std::sqrt(sqr_magnitude());
        }

        void normalize() {
            *this = this->normalized();
        }

        Vector normalized() const {
            Vector copy = *this;
            copy /= magnitude();
            return copy;
        }

        std::array<float, D> data() const
        {
            return data;
        }

        friend std::ostream& operator<<(std::ostream &out, const Vector<T, D> &vec) {
            out << "Vec( ";
            for (auto i = 0; i < D; i++)
                out << vec[i] << " ";
            out << ")";
            return out;
        }


    private:
        std::array<float, D> data_;
    };


    template<typename T, size_t D>
    inline Vector<T, D> operator+(Vector<T, D> lhs, const Vector<T, D> &rhs) {
        lhs += rhs;
        return lhs;
    }

    template<typename T, size_t D>
    inline Vector<T, D> operator-(Vector<T, D> lhs, const Vector<T, D> &rhs) {
        lhs -= rhs;
        return lhs;
    }


    template<typename T, size_t D>
    inline float operator*(const Vector<T, D> &lhs, const Vector<T, D> &rhs) {
        float acc = 0;
        for (unsigned i = 0; i < D; i++)
            acc += lhs[i] * rhs[i];
        return acc;
    }


    template<typename T, size_t D>
    inline Vector<T, D> operator^(Vector<T, D> lhs, Vector<T, D> const& rhs) {
        lhs ^= rhs;
        return lhs;
    }

    template<typename T, size_t D>
    inline Vector<T, D> operator*(const float &l, Vector<T, D> v) {
        return v *= l;
    }

    template<typename T, size_t D>
    inline Vector<T, D> operator*(Vector<T, D> v, const float &l) {
        return v *= l;
    }

    template<typename T, size_t D>
    inline Vector<T, D> operator/(const float &l, Vector<T, D> v) {
        return v /= l;
    }

    template<typename T, size_t D>
    inline Vector<T, D> operator/(Vector<T, D> v, const float &l) {
        return v /= l;
    }

    using Vec2 = Vector<float, 2>;
    using Vec3 = Vector<float, 3>;
    using Vec4 = Vector<float, 4>;

    template<> inline Vec3& Vec3::operator^=(const Vector& rhs) {
        float x = data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1];
        float y = data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2];
        float z = data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0];
        data_[0] = x;
        data_[1] = y;
        data_[2] = z;
        return *this;
    }


}

namespace YAML {
    template<>
    struct convert<mygl::Vec3> {
        static bool decode(const Node& node, mygl::Vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs[0] = node[0].as<double>();
            rhs[1] = node[1].as<double>();
            rhs[2] = node[2].as<double>();
            return true;
        }
    };
}
