//
// Created by mattrouss on 4/11/20.
//

#include "vec3.h"


namespace mygl {
    float Vec3::operator[](size_t idx) const {
        return data_[idx];
    }

    float &Vec3::operator[](size_t idx){
        return data_[idx];
    }

    Vec3& Vec3::operator+=(const Vec3 &v) {
        for (unsigned i = 0; i < 3; i++)
            data_[i] += v[i];

        return *this;
    }

    Vec3& Vec3::operator-=(const Vec3 &v) {
        for (unsigned i = 0; i < 3; i++)
            data_[i] -= v[i];

        return *this;
    }

    Vec3& Vec3::operator*=(const float &l) {
        for (unsigned i = 0; i < 3; i++)
            data_[i] *= l;

        return *this;
    }

    Vec3&  Vec3::operator/=(const float &l) {
        for (unsigned i = 0; i < 3; i++)
            data_[i] /= l;

        return *this;
    }

    Vec3& Vec3::operator^=(const Vec3& rhs) {
        float x = data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1];
        float y = data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2];
        float z = data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0];
        data_[0] = x;
        data_[1] = y;
        data_[2] = z;
        return *this;
    }


    Vec3 operator*(const float &l, Vec3 v) {
        return v *= l;
    }

    Vec3 operator*(Vec3 v, const float &l) {
        return v *= l;
    }

    Vec3 operator/(const float &l, Vec3 v) {
        return v /= l;
    }

    Vec3 operator/(Vec3 v, const float &l) {
        return v /= l;
    }


    float Vec3::sqr_magnitude() const {
        return (*this) * (*this);
    }

    float Vec3::magnitude() const {
        return std::sqrt(sqr_magnitude());
    }

    Vec3 Vec3::normalized() const {
        Vec3 copy = *this;
        copy /= magnitude();
        return copy;
    }


    std::ostream& operator<<(std::ostream &out, const Vec3 &vec) {
        return out << "Vec3(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
    }
}
