#ifndef BUMP_LIGHT_H
#define BUMP_LIGHT_H

#include <memory>

#include "mygl/movable.h"
#include "utils/vector.h"
#include "utils/matrix4.h"

namespace LightType {
    enum: unsigned int {
        UNIFORM,
        DIRECTIONAL,
        AMBIENT
    };
}

struct Light : public Movable
{
    Light() = default;
    Light(mygl::Vec3 pos, mygl::Vec3 color, unsigned type, bool used = false);

    mygl::Vec3 pos;
    mygl::Vec3 color;
    const unsigned type = LightType::UNIFORM;
    bool used = false;
    //add strength attribute

    void use()
    {
        used = true;
    }
    void translate(mygl::Vec3 v) override;
    void set_pos(mygl::Vec3 p) override;
    void rotate(mygl::Vec3) override {}
    void set_rot(mygl::Vec3) override {}
    mygl::Vec3 get_pos() override
    {
        return pos;
    }


    unsigned get_type() {
        return type;
    }
};

struct UniformLight : public Light
{
    UniformLight() = default;
    UniformLight(mygl::Vec3 pos, mygl::Vec3 color, float intenity);

    float intensity;

    float get_intensity() const;
};

struct DirectionalLight : public Light
{
    DirectionalLight() = default;
    DirectionalLight(mygl::Vec3 pos, mygl::Vec3 color, mygl::Vec3 target);

    mygl::Vec3 target;

    mygl::Vec3 get_dir() const;
};

struct AmbientLight : public Light
{
    AmbientLight() = default;
    AmbientLight(mygl::Vec3 pos, mygl::Vec3 color);
};

#endif //BUMP_LIGHT_H
