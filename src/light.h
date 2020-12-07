#ifndef BUMP_LIGHT_H
#define BUMP_LIGHT_H

#include <memory>
#include "utils/vector.h"
#include "utils/matrix4.h"
#include "program.h"
#include "movable.h"

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

class LightManager
{
public:
    LightManager();

    void set_uniform(size_t i, mygl::Vec3 pos, mygl::Vec3 color, float intensity);
    void set_directional(size_t i, mygl::Vec3 pos, mygl::Vec3 target, mygl::Vec3 color);
    void set_ambient(size_t i, mygl::Vec3 pos, mygl::Vec3 color);
    void reset(size_t i);
    std::shared_ptr<Light> get(size_t i);

    void set_lights_uniform(mygl::program* prog);

private:
    std::array<std::shared_ptr<Light>, 16> lights;
};

#endif //BUMP_LIGHT_H
